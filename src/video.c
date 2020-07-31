#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <SDL.h>

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
  uint32_t embeddedFontData[] = {
    #include "embed/font.inl"
  };
  memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
  memcpy(video->embeddedFont, embeddedFontData, sizeof(embeddedFontData));
  resetDrawState(video);
}

void disableVideo(Video *video) {
  memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
}

// TODO: Remove SDL and write custom loader.
Bitmap *loadBitmap(const char *filePath) {
  SDL_Surface *surface = SDL_LoadBMP(filePath);
  if (surface == NULL) return NULL;
  int length = surface->w * surface->h;
  Bitmap *bitmap = (Bitmap*)malloc(sizeof(Bitmap));
  bitmap->width = surface->w;
  bitmap->height = surface->h;
  bitmap->pixels = (uint32_t*)malloc(length * sizeof(uint32_t));
  uint8_t *surfacePixels = (uint8_t *) surface->pixels;
  for (int i = 0; i < length; i++) {
    uint32_t b = *surfacePixels++;
    uint32_t g = *surfacePixels++;
    uint32_t r = *surfacePixels++;
    bitmap->pixels[i] = (r << 16) | (g << 8) | (b);
  }
  SDL_FreeSurface(surface);
  return bitmap;
}

void freeBitmap(Bitmap *bitmap) {
  free(bitmap->pixels);
  free(bitmap);
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

void clip(Video *video, int x, int y, int w, int h) {
  video->clipRect = (Rect) {
    .left = CLAMP(x, 0, FRAMEBUFFER_WIDTH),
    .right = CLAMP(x + w, 0, FRAMEBUFFER_WIDTH),
    .top = CLAMP(y, 0, FRAMEBUFFER_HEIGHT),
    .bottom = CLAMP(y + h, 0, FRAMEBUFFER_HEIGHT)
  };
}

#define FRAMEBUFFER_POS(x, y) ((FRAMEBUFFER_WIDTH * y) + x)

void clearFramebuffer(Video *video, uint32_t color) {
  Rect clip = video->clipRect;
  size_t width = (clip.right - clip.left) * sizeof(uint32_t);
  for (int y = clip.top; y < clip.bottom; y++)
    memset(&video->framebuffer[FRAMEBUFFER_POS(clip.left, y)], color, width);
}

void pixel(Video *video, int x, int y, uint32_t color) {
  Rect clip = video->clipRect;
  if (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)
    video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

void line(Video *video, int x0, int y0, int x1, int y1, uint32_t color) {
  int xDistance = x1 - x0;
  int yDistance = y1 - y0;
  int xStep = SIGN(xDistance);
  int yStep = SIGN(yDistance);
  xDistance = abs(xDistance) << 1;
  yDistance = abs(xDistance) << 1;
  pixel(video, x0, y0, color);
  if (xDistance > yDistance) {
    int fraction = yDistance - (xDistance >> 1);
    while (x0 != x1) {
      x0 += xStep;
      if (fraction >= 0) {
        y0 += yStep;
        fraction -= xDistance;
      }
      fraction += yDistance;
      pixel(video, x0, y0, color);
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
      pixel(video, x0, y0, color);
    }
  }
}

static void horizontalLine(Video *video, int x, int y, int w, uint32_t color) {
  for (int i = x; i <= x + w; i++)
    pixel(video, i, y, color);
}

static void verticalLine(Video *video, int x, int y, int h, uint32_t color) {
  for (int i = y; i <= y + h; i++)
    pixel(video, x, i, color);
}

void rect(Video *video, int x, int y, int w, int h, uint32_t color) {
  horizontalLine(video, x, y, w, color);     // Top edge
  horizontalLine(video, x, y + h, w, color); // Bottom edge
  verticalLine(video, x, y, h, color);       // Left edge
  verticalLine(video, x + w, y, h, color);   // Right edge
}

void rectFill(Video *video, int x, int y, int w, int h, uint32_t color) {
  for (int i = y; i < y + h; i++) {
    for (int j = x; j < x + w; j++)
      pixel(video, j, i, color);
  }
}

static void blitBuffer(Video *video, uint32_t *pixels, int x, int y, int w, int h, int pitch, float scale, uint8_t flip, uint32_t color, ColorMode mode) {
  scale = CLAMP(scale, 0.1, MAX_SCALE);
  float width = w * scale;
  float height = h * scale;
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
      uint32_t col = pixels[yNearest * pitch + xNearest];
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
        pixel(video, xDest, yDest, col);
      }
    }
  }
}

void sprite(Video *video, Bitmap *bmp, int index, int x, int y, int w, int h, float scale, uint8_t flip, uint32_t color, ColorMode mode) {
  if (x >= 0 && y >= 0 && x <= FRAMEBUFFER_WIDTH && y <= FRAMEBUFFER_HEIGHT) {
    int numRows = bmp->height / 8;
    int numColumns = bmp->width / 8;
    int row = FLOOR(index / numColumns);
    int column = FLOOR(index % numColumns);
    w = CLAMP(w, 1, numColumns - column);
    h = CLAMP(h, 1, numRows - row);
    int widthPx = w * 8;
    int heightPx = h * 8;
    int yPx = row * bmp->width * 8;
    int xPx = column * 8;
    uint32_t *pixels = &bmp->pixels[yPx + xPx];
    blitBuffer(video, pixels, x, y, widthPx, heightPx, bmp->width, scale, flip, color, mode);
  }
}

#define IS_ASCII(c) (0 < c)

void font(Video *video, Bitmap *bmp, int x, int y, const char *str, int scale, uint32_t color) {
  int xCurrent = x;
  int yCurrent = y;
  char curr;
  uint32_t *fontPixels;
  int numColumns;
  if (!bmp) {
    fontPixels = video->embeddedFont;
    numColumns = 12;
  } else {
    fontPixels = bmp->pixels;
    numColumns = bmp->width / 8;
  }
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
          int yPixel = FLOOR((curr - 33) / numColumns) * FONT_WIDTH * FONT_CHAR_HEIGHT;
          int xPixel = FLOOR((curr - 33) % numColumns) * FONT_CHAR_WIDTH;
          uint32_t *pixels = &fontPixels[yPixel + xPixel];
          blitBuffer(video, pixels, xCurrent, yCurrent, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT, FONT_WIDTH, scale, 0, color, Solid);
          xCurrent += (FONT_CHAR_WIDTH) * scale;
        } break;
    }
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
