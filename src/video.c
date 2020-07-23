#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"
#include "video.h"

#define MIN_SCALE 1
#define MAX_SCALE 5

#define RMASK 0x00ff0000
#define GMASK 0x0000ff00
#define BMASK 0x000000ff

#define r_comp(color) ((color & RMASK) >> 16)
#define g_comp(color) ((color & GMASK) >> 8)
#define b_comp(color) ((color & BMASK))

#define AVERAGE_COMP(c1, c2) (MIN((c1 + c2) / 2, 255))
#define ADD_COMP(c1, c2) (MIN((c1 + c2), 255))

#define AVERAGE_COLORS(col1, col2)\
  ((AVERAGE_COMP(r_comp(col1), r_comp(col2)) << 16) | (AVERAGE_COMP(g_comp(col1), g_comp(col2)) << 8) | AVERAGE_COMP(b_comp(col1), b_comp(col2)))

#define ADD_COLORS(col1, col2)\
  ((ADD_COMP(r_comp(col1), r_comp(col2)) << 16) | (ADD_COMP(g_comp(col1), g_comp(col2)) << 8) | ADD_COMP(b_comp(col1), b_comp(col2)))

void initializeVideo(Video *video) {
  Color32 embeddedFontData[] = {
    #include "embed/font.inl"
  };
  memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
  memset(&video->spriteSheet, 0x00, sizeof(video->spriteSheet));
  memset(&video->fonts, 0x00, sizeof(video->fonts));
  memcpy(video->embeddedFont, embeddedFontData, sizeof(embeddedFontData));
  resetDrawState(video);
}

void disableVideo(Video *video) {
  memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
  memset(&video->spriteSheet, 0x00, sizeof(video->spriteSheet));
  memset(&video->fonts, 0x00, sizeof(video->fonts));
}

void loadSpriteSheet(Video *video, const char *path) {
  video->loadBMP(path, video->spriteSheet, sizeof(video->spriteSheet) / sizeof(Color32));
}

void loadFont(Video *video, int id, const char *path) {
  video->loadBMP(path, video->fonts[id], sizeof(video->fonts[id]) / sizeof(Color32));
}

void resetDrawState(Video *video) {
  video->colorKey = 0x00;
  video->clipRect = (Rect) {
    .top = 0,
    .left = 0,
    .bottom = FRAMEBUFFER_HEIGHT,
    .right = FRAMEBUFFER_WIDTH
  };
}

void setClippingRect(Video *video, int x, int y, int w, int h) {
  video->clipRect = (Rect) {
    .left = CLAMP(x, 0, FRAMEBUFFER_WIDTH),
    .right = CLAMP(x + w, 0, FRAMEBUFFER_WIDTH),
    .top = CLAMP(y, 0, FRAMEBUFFER_HEIGHT),
    .bottom = CLAMP(y + h, 0, FRAMEBUFFER_HEIGHT)
  };
}

#define FRAMEBUFFER_POS(x, y) ((FRAMEBUFFER_WIDTH * y) + x)

void clearFramebuffer(Video *video, Color32 color) {
  Rect clip = video->clipRect;
  for (int y = clip.top; y < clip.bottom; y++) {
    for (int x = clip.left; x < clip.right; x++)
      video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
  }
}

void blitPixel(Video *video, int x, int y, Color32 color) {
  Rect clip = video->clipRect;
  if (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)
    video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color) {
  int xDistance = x1 - x0;
  int yDistance = y1 - y0;
  int xStep = SIGN(xDistance);
  int yStep = SIGN(yDistance);
  xDistance = abs(xDistance) << 1;
  yDistance = abs(xDistance) << 1;
  blitPixel(video, x0, y0, color);
  if (xDistance > yDistance) {
    int fraction = yDistance - (xDistance >> 1);
    while (x0 != x1) {
      x0 += xStep;
      if (fraction >= 0) {
        y0 += yStep;
        fraction -= xDistance;
      }
      fraction += yDistance;
      blitPixel(video, x0, y0, color);
    }
  } else {
    int fraction = xDistance - (yDistance >> 1);
    while (y0 != y1) {
      if (fraction >= 0) {
        x0 += xStep;
        fraction -= yDistance;
      }
      y0 += yStep;
      fraction += xDistance;
      blitPixel(video, x0, y0, color);
    }
  }
}

static void horizontalLine(Video *video, int x, int y, int w, Color32 color) {
  for (int i = x; i <= x + w; i++)
    blitPixel(video, i, y, color);
}

static void verticalLine(Video *video, int x, int y, int h, Color32 color) {
  for (int i = y; i <= y + h; i++)
    blitPixel(video, x, i, color);
}

void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color) {
  horizontalLine(video, x, y, w, color);     // Top edge
  horizontalLine(video, x, y + h, w, color); // Bottom edge
  verticalLine(video, x, y, h, color);       // Left edge
  verticalLine(video, x + w, y, h, color);   // Right edge
}

void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color) {
  for (int i = y; i < y + h; i++) {
    for (int j = x; j < x + w; j++)
      blitPixel(video, j, i, color);
  }
}

static void blitBuffer(Video *video, const Color32 *pixels, int x, int y, int w, int h, int pitch, int scale, uint8_t flip, Color32 color, ColorMode mode) {
  scale = CLAMP(scale, MIN_SCALE, MAX_SCALE);
  int width = w * scale;
  int height = h * scale;
  int xRatio = FLOOR((w << 16) / width + 0.5);
  int yRatio = FLOOR((h << 16) / height + 0.5);
  int xPixelStart = IS_BIT_SET(flip, 1) ? width - 1 : 0;
  int yPixelStart = IS_BIT_SET(flip, 2) ? height - 1 : 0;
  int xStep = IS_BIT_SET(flip, 1) ? -1 : 1;
  int yStep = IS_BIT_SET(flip, 2) ? -1 : 1;
  int xSource, ySource, xDest, yDest;
  for (yDest = y, ySource = yPixelStart; yDest < y + height; yDest++, ySource += yStep) {
    for (xDest = x, xSource = xPixelStart; xDest < x + width; xDest++, xSource += xStep) {
      int xNearest = (xSource * xRatio) >> 16;
      int yNearest = (ySource * yRatio) >> 16;
      Color32 col = pixels[yNearest * pitch + xNearest];
      if (col != video->colorKey) {
        switch(mode) {
          case Average:
            col = AVERAGE_COLORS(col, color); break;
          case Additive:
            col = ADD_COLORS(col, color); break;
          case Solid:
            col = color; break;
          default: break;
        }
        blitPixel(video, xDest, yDest, col);
      }
    }
  }
}

#define SPRITE_SHEET_CELLS ((int)(SPRITE_SHEET_SQRSIZE / SPRITE_SQRSIZE))

void blitSprite(Video *video, int id, int x, int y, int w, int h, int scale, uint8_t flip, Color32 color, ColorMode mode) {
  if (id >= 0 && id < SPRITE_SHEET_SQRSIZE) {
    int row = FLOOR(id / SPRITE_SHEET_CELLS);
    int column = FLOOR(id % SPRITE_SHEET_CELLS);
    w = CLAMP(w, 1, SPRITE_SHEET_CELLS - column);
    h = CLAMP(h, 1, SPRITE_SHEET_CELLS - row);
    int widthPx = w * SPRITE_SQRSIZE;
    int heightPx = h * SPRITE_SQRSIZE;
    int yPx = row * SPRITE_SHEET_SQRSIZE * SPRITE_SQRSIZE;
    int xPx = column * SPRITE_SQRSIZE;
    Color32 *pixels = &video->spriteSheet[yPx + xPx];
    blitBuffer(video, pixels, x, y, widthPx, heightPx, SPRITE_SHEET_SQRSIZE, scale, flip, color, mode);
  }
}

int fontWidth(const char *text) {
  int currentWidth = 0, width = 0;
  char curr;
  while ((curr = *text++)) {
    switch (curr) {
      case ' ':
        currentWidth += FONT_CHAR_SPACING; break;
      case '\n':
        currentWidth = 0; break;
      default:
        currentWidth += FONT_CHAR_WIDTH; break;
    }
    if (currentWidth > width)
      width = currentWidth;
  }
  return width;
}

#define IS_ASCII(c) (0 < c)
#define FONT_COLUMNS (FONT_WIDTH / FONT_CHAR_WIDTH)

void blitFont(Video *video, int id, int x, int y, const char *str, int scale, Color32 color) {
  Color32 *fontPixels = id == -1 ? video->embeddedFont : video->fonts[id];
  int xCurrent = x;
  int yCurrent = y;
  char curr;
  while ((curr = *str++)) {
    if (!IS_ASCII(curr))
      curr = '?';
    switch (curr) {
      case '\n':
        xCurrent = x;
        yCurrent += FONT_CHAR_HEIGHT * scale; break;
      case ' ':
        xCurrent += FONT_CHAR_SPACING * scale; break;
      default: {
          int yPixel = FLOOR((curr - 33) / FONT_COLUMNS) * FONT_WIDTH * FONT_CHAR_HEIGHT;
          int xPixel = FLOOR((curr - 33) % FONT_COLUMNS) * FONT_CHAR_WIDTH;
          Color32 *pixels = &fontPixels[yPixel + xPixel];
          blitBuffer(video, pixels, xCurrent, yCurrent, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT, FONT_WIDTH, scale, 0, color, Solid);
          xCurrent += (FONT_CHAR_WIDTH) * scale;
        } break;
    }
  }
}
