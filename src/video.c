#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "video.h"

#define MIN_SCALE 1
#define MAX_SCALE 5

void initializeVideo(Video *video)
{
  Color32 embeddedFontData[] =
  {
    #include "embed/font.inl"
  };

  memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
  memset(&video->spriteSheet, 0x00, sizeof(video->spriteSheet));
  memset(&video->fonts, 0x00, sizeof(video->fonts));
  memcpy(video->embeddedFont, embeddedFontData, sizeof(embeddedFontData));

  resetDrawState(video);
}

void disableVideo(Video *video)
{
  memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
  memset(&video->spriteSheet, 0x00, sizeof(video->spriteSheet));
  memset(&video->fonts, 0x00, sizeof(video->fonts));
}

void loadSpriteSheet(Video *video, const char *path)
{
  video->loadBMP(path, video->spriteSheet, sizeof(video->spriteSheet) / sizeof(Color32));
}

void loadFont(Video *video, int id, const char *path)
{
  video->loadBMP(path, video->fonts[id], sizeof(video->fonts[id]));
}

void resetDrawState(Video *video)
{
  video->colorKey = 0x00;

  video->clipRect = (Rect)
  {
    .top = 0,
    .left = 0,
    .bottom = FRAMEBUFFER_HEIGHT,
    .right = FRAMEBUFFER_WIDTH
  };
}

void setClippingRect(Video *video, int x, int y, int w, int h)
{
  video->clipRect = (Rect)
  {
    .left = CLAMP(x, 0, FRAMEBUFFER_WIDTH),
    .right = CLAMP(x + w, 0, FRAMEBUFFER_WIDTH),
    .top = CLAMP(y, 0, FRAMEBUFFER_HEIGHT),
    .bottom = CLAMP(y + h, 0, FRAMEBUFFER_HEIGHT)
  };
}

#define FRAMEBUFFER_POS(x, y) ((FRAMEBUFFER_WIDTH * y) + x)

void clearFramebuffer(Video *video, Color32 color)
{
  Rect clip = video->clipRect;

  for (int y = clip.top; y < clip.bottom; y++)
  {
    for (int x = clip.left; x < clip.right; x++)
      video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
  }
}

void blitPixel(Video *video, int x, int y, Color32 color)
{
  Rect clip = video->clipRect;

  if (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)
    video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color)
{
  int xDistance = x1 - x0;
  int yDistance = y1 - y0;
  int xStep = SIGN(xDistance);
  int yStep = SIGN(yDistance);

  xDistance = abs(xDistance) << 1;
  yDistance = abs(xDistance) << 1;

  blitPixel(video, x0, y0, color);

  if (xDistance > yDistance)
  {
    int fraction = yDistance - (xDistance >> 1);

    while (x0 != x1)
    {
      x0 += xStep;

      if (fraction >= 0)
      {
        y0 += yStep;
        fraction -= xDistance;
      }

      fraction += yDistance;
      blitPixel(video, x0, y0, color);
    }
  }
  else
  {
    int fraction = xDistance - (yDistance >> 1);

    while (y0 != y1)
    {
      if (fraction >= 0)
      {
        x0 += xStep;
        fraction -= yDistance;
      }

      y0 += yStep;
      fraction += xDistance;
      blitPixel(video, x0, y0, color);
    }
  }
}

static void horizontalLine(Video *video, int x, int y, int w, Color32 color)
{
  for (int i = x; i <= x + w; i++)
    blitPixel(video, i, y, color);
}

static void verticalLine(Video *video, int x, int y, int h, Color32 color)
{
  for (int i = y; i <= y + h; i++)
    blitPixel(video, x, i, color);
}

void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
  horizontalLine(video, x, y, w, color);     // Top edge
  horizontalLine(video, x, y + h, w, color); // Bottom edge
  verticalLine(video, x, y, h, color);       // Left edge
  verticalLine(video, x + w, y, h, color);   // Right edge
}

void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
  for (int i = y; i < y + h; i++)
  {
    for (int j = x; j < x + w; j++)
      blitPixel(video, j, i, color);
  }
}

static void blitRect(Video *video, const Color32 *pixels, int x, int y, int w, int h, int pitch, int scale, u8 flip, const Color32 *color)
{
  scale = CLAMP(scale, MIN_SCALE, MAX_SCALE);

  int width = w * scale;
  int height = h * scale;
  int xRatio = FLOOR((w << 16) / width + 0.5);
  int yRatio = FLOOR((w << 16) / width + 0.5);

  bool isXFlipped = IS_BIT_SET(flip, 1);
  bool isYFlipped = IS_BIT_SET(flip, 2);
  int xPixelStart = isXFlipped ? width - 1 : 0;
  int yPixelStart = isYFlipped ? height - 1 : 0;

  int xStep = isXFlipped ? -1 : 1;
  int yStep = isYFlipped ? -1 : 1;

  int xPixel, yPixel, xFramebuffer, yFramebuffer;

  for (yFramebuffer = y, yPixel = yPixelStart; yFramebuffer < y + height; yFramebuffer++, yPixel += yStep)
  {
    for (xFramebuffer = x, xPixel = xPixelStart; xFramebuffer < x + width; xFramebuffer++, xPixel += xStep)
    {
      int xNearest = (xPixel * xRatio) >> 16;
      int yNearest = (yPixel * yRatio) >> 16;

      Color32 col = pixels[yNearest * pitch + xNearest];

      if (col != video->colorKey)
        blitPixel(video, xFramebuffer, yFramebuffer, color != NULL ? *color : col);
    }
  }
}

#define SPRSHEET_COLUMNS ((int)(SPRITE_SHEET_SQRSIZE / SPRITE_SQRSIZE))
#define SPRSHEET_ROW_SIZE ((int)(SPRITE_SHEET_SQRSIZE * SPRITE_SQRSIZE))
#define SPRSHEET_POS(x, y) (y * SPRSHEET_ROW_SIZE + x * SPRITE_SQRSIZE)

void blitSprite(Video *video, int id, int x, int y, int w, int h, int scale, u8 flip)
{
  if (id >= 0 && id < SPRITE_SHEET_SQRSIZE)
  {
    int width = w * SPRITE_SQRSIZE;
    int height = h * SPRITE_SQRSIZE;
    int row = FLOOR(id / SPRSHEET_COLUMNS);
    int col = FLOOR(id % SPRSHEET_COLUMNS);

    Color32 *pixels = &video->spriteSheet[SPRSHEET_POS(col, row)];

    blitRect(video, pixels, x, y, width, height, SPRITE_SHEET_SQRSIZE, scale, flip, NULL);
  }
}

int fontWidth(const char *text)
{
  int currentWidth = 0;
  int width = 0;
  char curr;

  while ((curr = *text++))
  {
    switch (curr)
    {
      case ' ':
        currentWidth += FONT_CHAR_SPACING;
        break;
      case '\n':
        currentWidth = 0;
        break;
      default:
        currentWidth += FONT_CHAR_WIDTH - FONT_CHAR_OFFSET;
        break;
    }

    if (currentWidth > width)
      width = currentWidth;
  }

  return width;
}

#define IS_ASCII(c) (0 < c)
#define FONT_COLUMNS (FONT_WIDTH / FONT_CHAR_WIDTH)

void blitFont(Video *video, int id, int x, int y, const char *str, int scale, Color32 color)
{
  Color32 *fontPixels = id == -1 ? video->embeddedFont : video->fonts[id];

  int xCurrent = x;
  int yCurrent = y;
  char curr;

  while ((curr = *str++))
  {
    if (!IS_ASCII(curr)) curr = '?';

    switch (curr)
    {
      case '\n':
        xCurrent = x;
        yCurrent += FONT_CHAR_HEIGHT * scale;
        break;
      case ' ':
        xCurrent += FONT_CHAR_SPACING * scale;
        break;
      default:
        {
          int row = FLOOR((curr - 33) / FONT_COLUMNS);
          int col = FLOOR((curr - 33) % FONT_COLUMNS);

          int yy = row * FONT_WIDTH * FONT_CHAR_HEIGHT;
          int xx = col * FONT_CHAR_WIDTH + FONT_CHAR_OFFSET;

          Color32 *pixels = &fontPixels[yy + xx];

          blitRect(video, pixels, xCurrent, yCurrent, FONT_CHAR_WIDTH - FONT_CHAR_OFFSET, FONT_CHAR_HEIGHT, FONT_WIDTH, scale, 0, &color);

          xCurrent += FONT_CHAR_WIDTH - FONT_CHAR_OFFSET * scale;
        }
        break;
    }
  }
}
