#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"
#include "video.h"

/**
 * Milk handles drawing, clipping, and blending pixels on a per pixel basis.
 * Although this isn't very efficient, it hasn't been much of an issue so far.
 *
 * Possible improvements:
 *  * Clip buffers before writing them to the framebuffer.
 *    Right now, clipping is applied per pixel, which isn't exactly efficient.
 *
 *  * RLE encode pixel buffers to process rows of pixels.
 *    Blending and blitting can be applied to an entire row at once.
 *
 * - ST
*/

#define EMBED_FONT_WIDTH 96
#define EMBED_FONT_HEIGHT 64

static uint32_t embeddedFontData[] =
    {
#include "embed/font.inl"
};

void initializeVideo(Video *video)
{
  memset(&video->framebuffer, 0, sizeof(video->framebuffer));
  resetDrawState(video);
}

void resetDrawState(Video *video)
{
  video->colorKey = 0xff000000;
  video->clipRect.top = 0;
  video->clipRect.left = 0;
  video->clipRect.bottom = FRAMEBUFFER_HEIGHT;
  video->clipRect.right = FRAMEBUFFER_WIDTH;
}

void setClip(Video *video, int x, int y, int w, int h)
{
  video->clipRect.left = CLAMP(x, 0, FRAMEBUFFER_WIDTH);
  video->clipRect.right = CLAMP(x + w, 0, FRAMEBUFFER_WIDTH);
  video->clipRect.top = CLAMP(y, 0, FRAMEBUFFER_HEIGHT);
  video->clipRect.bottom = CLAMP(y + h, 0, FRAMEBUFFER_HEIGHT);
}

#define FRAMEBUFFER_POS(x, y) (y * FRAMEBUFFER_WIDTH + x)

void clearFramebuffer(Video *video, uint32_t color)
{
  Rect clip = video->clipRect;
  int length = clip.right - clip.left;

  for (int y = clip.top; y < clip.bottom; y++)
  {
    int start = FRAMEBUFFER_POS(clip.left, y);
    int end = start + length;

    for (int x = start; x < end; x++)
      video->framebuffer[x] = color;
  }
}

void drawPixel(Video *video, int x, int y, uint32_t color)
{
  if (video->clipRect.left <= x && x < video->clipRect.right && video->clipRect.top <= y && y < video->clipRect.bottom)
    video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

void drawLine(Video *video, int x0, int y0, int x1, int y1, uint32_t color)
{
  int xDistance = x1 - x0;
  int yDistance = y1 - y0;
  int xStep = SIGN(xDistance);
  int yStep = SIGN(yDistance);
  xDistance = abs(xDistance) << 1;
  yDistance = abs(xDistance) << 1;

  drawPixel(video, x0, y0, color);

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
      drawPixel(video, x0, y0, color);
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
      drawPixel(video, x0, y0, color);
    }
  }
}

static void __horizontalLine(Video *video, int x, int y, int w, uint32_t color)
{
  for (int i = x; i <= x + w; i++)
    drawPixel(video, i, y, color);
}

static void __verticalLine(Video *video, int x, int y, int h, uint32_t color)
{
  for (int i = y; i <= y + h; i++)
    drawPixel(video, x, i, color);
}

void drawRect(Video *video, int x, int y, int w, int h, uint32_t color)
{
  __horizontalLine(video, x, y, w, color);     // Top edge
  __horizontalLine(video, x, y + h, w, color); // Bottom edge
  __verticalLine(video, x, y, h, color);       // Left edge
  __verticalLine(video, x + w, y, h, color);   // Right edge
}

void drawFilledRect(Video *video, int x, int y, int w, int h, uint32_t color)
{
  for (int i = y; i < y + h; i++)
    for (int j = x; j < x + w; j++)
      drawPixel(video, j, i, color);
}

#define A_COMP(color) ((color & 0xff000000) >> 24)
#define R_COMP(color) ((color & 0x00ff0000) >> 16)
#define G_COMP(color) ((color & 0x0000ff00) >> 8)
#define B_COMP(color) ((color & 0x000000ff))

#define NORM(val, min, max) ((val - min) / (max - min))

#define BLEND(dest, src)                                                    \
  do                                                                        \
  {                                                                         \
    double normalA = (double)A_COMP(src) / 255;                             \
    unsigned char r = R_COMP(src) * normalA + R_COMP(dest) * (1 - normalA); \
    unsigned char g = G_COMP(src) * normalA + G_COMP(dest) * (1 - normalA); \
    unsigned char b = B_COMP(src) * normalA + B_COMP(dest) * (1 - normalA); \
    dest = (255 << 24) | (r << 16) | (g << 8) | b;                          \
  } while (0)

static void __drawBuffer(Video *video, uint32_t *buffer, int x, int y, int w, int h, int pitch, float scale, uint8_t flip, uint32_t color)
{
  if (scale <= 0)
    return;

  float scaledWidth = w * scale;
  float scaledHeight = h * scale;

  int xStep = CHECK_BIT(flip, 1) ? -1 : 1;
  int yStep = CHECK_BIT(flip, 2) ? -1 : 1;
  int ySourceStart = CHECK_BIT(flip, 2) ? scaledHeight - 1 : 0;
  int xSourceStart = CHECK_BIT(flip, 1) ? scaledWidth - 1 : 0;
  int yDestEnd = FLOOR(y + scaledHeight);
  int xDestEnd = FLOOR(x + scaledWidth);
  int xRatio = FLOOR((w << 16) / scaledWidth + 0.5f);
  int yRatio = FLOOR((h << 16) / scaledHeight + 0.5f);

  for (int yDest = y, ySource = ySourceStart; yDest < yDestEnd; yDest++, ySource += yStep)
  {
    for (int xDest = x, xSource = xSourceStart; xDest < xDestEnd; xDest++, xSource += xStep)
    {
      int xNearest = (xSource * xRatio) >> 16;
      int yNearest = (ySource * yRatio) >> 16;
      uint32_t pixel = buffer[yNearest * pitch + xNearest];

      if (pixel != video->colorKey)
      {
        BLEND(pixel, color);
        drawPixel(video, xDest, yDest, pixel);
      }
    }
  }
}

#define BUFFER_CHUNK(bmp, row, column) (&bmp.pixels[row * bmp.width * SPRITE_SIZE + column * SPRITE_SIZE])

void drawSprite(Video *video, Bitmap *bmp, int index, int x, int y, int w, int h, float scale, uint8_t flip, uint32_t color)
{
  int numRows = bmp->height / SPRITE_SIZE;
  int numColumns = bmp->width / SPRITE_SIZE;
  int row = FLOOR(index / numColumns);
  int column = FLOOR(index % numColumns);

  w = CLAMP(w, 1, numColumns - column);
  h = CLAMP(h, 1, numRows - row);

  __drawBuffer(video, BUFFER_CHUNK((*bmp), row, column), x, y, w * SPRITE_SIZE, h * SPRITE_SIZE, bmp->width, scale, flip, color);
}

#define IS_ASCII(c) (0 < c)

void drawFont(Video *video, Bitmap *bmp, int x, int y, const char *text, int scale, uint32_t color)
{
  if (!text || scale <= 0)
    return;

  Bitmap bitmap = bmp != NULL ? *bmp : (Bitmap){.pixels = embeddedFontData, .width = EMBED_FONT_WIDTH, .height = EMBED_FONT_HEIGHT};

  int numColumns = bitmap.width / SPRITE_SIZE;
  int xCurrent = x;
  int yCurrent = y;
  char curr;

  while ((curr = *text))
  {
    if (!IS_ASCII(curr))
      curr = '?';

    switch (curr)
    {
    case '\n':
      xCurrent = x;
      yCurrent += SPRITE_SIZE * scale;
      break;
    case ' ':
      xCurrent += FONT_SPRITE_SPACING * scale;
      break;
    default:
    {
      int row = FLOOR((curr - 33) / numColumns);
      int col = FLOOR((curr - 33) % numColumns);

      __drawBuffer(video, BUFFER_CHUNK(bitmap, row, col), xCurrent, yCurrent, SPRITE_SIZE, SPRITE_SIZE, bitmap.width, scale, 0, color);

      xCurrent += SPRITE_SIZE * scale;
    }
    break;
    }
    text++;
  }
}

static int __wrapSeek(const char *text, int maxLength)
{
  int lineLength = 0;
  int breakLength = 0;
  char c;

  while ((c = *text) && lineLength < maxLength)
  {
    text++;
    lineLength++;

    if (c == ' ')
      breakLength = lineLength;
  }

  return breakLength == 0 || !c ? lineLength : breakLength;
}

void drawWrappedFont(Video *video, Bitmap *bmp, int x, int y, int w, const char *text, int scale, uint32_t color)
{
  if (!text || scale <= 0 || w < SPRITE_SIZE)
    return;

  Bitmap bitmap = bmp != NULL ? *bmp : (Bitmap){.pixels = embeddedFontData, .width = EMBED_FONT_WIDTH, .height = EMBED_FONT_HEIGHT};

  int numColumns = bitmap.width / SPRITE_SIZE;
  int maxLineLength = FLOOR(w / SPRITE_SIZE);
  int yCurrent = y;

  while (*text)
  {
    int length = __wrapSeek(text, maxLineLength);
    int xCurrent = x;

    for (int i = 0; i < length; i++, text++)
    {
      char c = *text;

      if (c == '\n' || (i == 0 && c == ' '))
        continue;

      if (c != ' ')
      {
        if (!IS_ASCII(c))
          c = '?';

        int row = FLOOR((c - 33) / numColumns);
        int col = FLOOR((c - 33) % numColumns);

        __drawBuffer(video, BUFFER_CHUNK(bitmap, row, col), xCurrent, yCurrent, SPRITE_SIZE, SPRITE_SIZE, bitmap.width, scale, 0, color);

        xCurrent += SPRITE_SIZE * scale;
      }
      else
        xCurrent += FONT_SPRITE_SPACING;
    }

    yCurrent += SPRITE_SIZE;
  }
}

int getFontWidth(const char *text)
{
  if (!text || *text == '\0')
    return 0;

  int currentWidth = 0, width = 0;
  char curr;

  while ((curr = *text))
  {
    switch (curr)
    {
    case ' ':
      currentWidth += FONT_SPRITE_SPACING;
      break;
    case '\n':
      currentWidth = 0;
      break;
    default:
      currentWidth += SPRITE_SIZE;
      break;
    }

    if (currentWidth > width)
      width = currentWidth;

    text++;
  }
  return width;
}
