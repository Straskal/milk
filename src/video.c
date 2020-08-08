#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>

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

#define RMASK 0x00ff0000
#define GMASK 0x0000ff00
#define BMASK 0x000000ff

#define r_comp(color) ((color & RMASK) >> 16)
#define g_comp(color) ((color & GMASK) >> 8)
#define b_comp(color) ((color & BMASK))

#define AVERAGE_COMP(c1, c2)  (MIN((c1 + c2) / 2, 255))
#define ADD_COMP(c1, c2)      (MIN((c1 + c2), 255))

#define BLEND_AVERAGE(col1, col2)\
  ((AVERAGE_COMP(r_comp(col1), r_comp(col2)) << 16) | (AVERAGE_COMP(g_comp(col1), g_comp(col2)) << 8) | AVERAGE_COMP(b_comp(col1), b_comp(col2)))

#define BLEND_ADDITIVE(col1, col2)\
  ((ADD_COMP(r_comp(col1), r_comp(col2)) << 16) | (ADD_COMP(g_comp(col1), g_comp(col2)) << 8) | ADD_COMP(b_comp(col1), b_comp(col2)))

#define EMBED_FONT_WIDTH  96
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
  video->colorKey         = 0;
  video->clipRect.top     = 0;
  video->clipRect.left    = 0;
  video->clipRect.bottom  = FRAMEBUFFER_HEIGHT;
  video->clipRect.right   = FRAMEBUFFER_WIDTH;
}

void setClip(Video *video, int x, int y, int w, int h)
{
  video->clipRect.left    = CLAMP(x, 0, FRAMEBUFFER_WIDTH);
  video->clipRect.right   = CLAMP(x + w, 0, FRAMEBUFFER_WIDTH);
  video->clipRect.top     = CLAMP(y, 0, FRAMEBUFFER_HEIGHT);
  video->clipRect.bottom  = CLAMP(y + h, 0, FRAMEBUFFER_HEIGHT);
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
  if (video->clipRect.left <= x  && x < video->clipRect.right && video->clipRect.top <= y && y < video->clipRect.bottom)
      video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

void drawLine(Video *video, int x0, int y0, int x1, int y1, uint32_t color)
{
  int xDistance = x1 - x0;
  int yDistance = y1 - y0;
  int xStep     = SIGN(xDistance);
  int yStep     = SIGN(yDistance);
  xDistance     = abs(xDistance) << 1;
  yDistance     = abs(xDistance) << 1;

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

#define BLEND_COLOR(color, blendColor, mode)\
  do {\
    switch (mode) {\
    case Average:   color = BLEND_AVERAGE(color, blendColor); break;\
    case Additive:  color = BLEND_ADDITIVE(color, blendColor); break;\
    case Solid:     color = blendColor; break;\
    default: break;\
    }\
  } while(0)

static void __drawBuffer(Video *video, uint32_t *buffer, int x, int y, int w, int h, int pitch, float scale, uint8_t flip, uint32_t color, ColorMode mode)
{
  if (scale <= 0)
    return;

  float width   = w * scale;
  float height  = h * scale;
  int xRatio    = FLOOR((w << 16) / width + 0.5f);
  int yRatio    = FLOOR((h << 16) / height + 0.5f);

  int xPixelStart   = CHECK_BIT(flip, 1) ? width - 1 : 0;
  int yPixelStart   = CHECK_BIT(flip, 2) ? height - 1 : 0;
  int xStep         = CHECK_BIT(flip, 1) ? -1 : 1;
  int yStep         = CHECK_BIT(flip, 2) ? -1 : 1;
  int yEnd          = FLOOR(y + height);
  int xEnd          = FLOOR(x + width);

  for (int yDest = y, ySource = yPixelStart; yDest < yEnd; yDest++, ySource += yStep)
  {
    for (int xDest = x, xSource = xPixelStart; xDest < xEnd; xDest++, xSource += xStep)
    {
      int xNearest = (xSource * xRatio) >> 16;
      int yNearest = (ySource * yRatio) >> 16;
      uint32_t col = buffer[yNearest * pitch + xNearest];

      if (col != video->colorKey)
      {
        BLEND_COLOR(col, color, mode);
        drawPixel(video, xDest, yDest, col);
      }
    }
  }
}

void drawSprite(Video *video, Bitmap *bmp, int index, int x, int y, int w, int h, float scale, uint8_t flip, uint32_t color, ColorMode mode)
{
  int numRows     = bmp->height / SPRITE_SIZE;
  int numColumns  = bmp->width / SPRITE_SIZE;
  int row         = FLOOR(index / numColumns);
  int column      = FLOOR(index % numColumns);

  w = CLAMP(w, 1, numColumns - column);
  h = CLAMP(h, 1, numRows - row);

  uint32_t *buffer = &bmp->pixels[row * bmp->width * SPRITE_SIZE + column * SPRITE_SIZE];

  __drawBuffer(video, buffer, x, y, w * SPRITE_SIZE, h * SPRITE_SIZE, bmp->width, scale, flip, color, mode);
}

#define GET_FONT_BUFFER(video, bmp, buffer, pitch, numColumns)\
  do {\
    if (!bmp) {\
      buffer = embeddedFontData;\
      pitch = EMBED_FONT_WIDTH;\
      numColumns = EMBED_FONT_WIDTH / SPRITE_SIZE;\
    } else {\
      buffer = bmp->pixels;\
      pitch = bmp->width;\
      numColumns = bmp->width / SPRITE_SIZE;\
    }\
  } while (0)

#define IS_ASCII(c) (0 < c)

void drawFont(Video *video, Bitmap *bmp, int x, int y, const char *text, int scale, uint32_t color)
{
  uint32_t *buffer;
  int pitch, numColumns;
  GET_FONT_BUFFER(video, bmp, buffer, pitch, numColumns);

  char curr;
  int xCurrent = x;
  int yCurrent = y;

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
      int yPixel = FLOOR((curr - 33) / numColumns) * pitch * SPRITE_SIZE;
      int xPixel = FLOOR((curr - 33) % numColumns) * SPRITE_SIZE;
      __drawBuffer(video, &buffer[yPixel + xPixel], xCurrent, yCurrent, SPRITE_SIZE, SPRITE_SIZE, pitch, scale, 0, color, Solid);
      xCurrent += SPRITE_SIZE * scale;
    }
    break;
    }
    text++;
  }
}

int drawWrappedFont(Video *video, Bitmap *bmp, int x, int y, int width, const char *text, int scale, uint32_t color) {
  if (!text || width < 1)
    return 0;

  uint32_t *buffer;
  int pitch, numColumns;
  GET_FONT_BUFFER(video, bmp, buffer, pitch, numColumns);

  int yCurrent = y;
  int maxLineLength = FLOOR(width / SPRITE_SIZE);

  while (*text)
  {
    const char *lineStart = text;
    const char *lineEnd   = NULL;
    int lineLength        = 0;
    char currChar;

    while ((currChar = *text))
    {
      text++;
      if (currChar == ' ')
        lineEnd = text;
      if (lineLength++ > maxLineLength - 1 || currChar == '\n')
        break;
    }
    if (!lineEnd)
      lineEnd = text;

    text = lineStart;
    int xCurrent = x;

    while (text != lineEnd)
    {
      currChar = *text++;

      if (currChar == '\n')
        continue;

      if (currChar != ' ')
      {
        if (!IS_ASCII(currChar))
          currChar = '?';

        int yPixel = FLOOR((currChar - 33) / numColumns) * pitch * SPRITE_SIZE;
        int xPixel = FLOOR((currChar - 33) % numColumns) * SPRITE_SIZE;

        __drawBuffer(video, &buffer[yPixel + xPixel], xCurrent, yCurrent, SPRITE_SIZE, SPRITE_SIZE, pitch, scale, 0, color, Solid);

        xCurrent += SPRITE_SIZE * scale;
      }
      else xCurrent += FONT_SPRITE_SPACING;
    }
    yCurrent += SPRITE_SIZE;
  }
  return (yCurrent - y) / SPRITE_SIZE;
}

int getFontWidth(const char *text)
{
  if (!text || *text == '\0')
    return 0;

  int currentWidth = 0, width = 0;
  char curr;

  while ((curr = *text++)) {
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
  }
  return width;
}

void flipFramebuffer(Video *video, uint32_t *frontBuffer) {
  memcpy(frontBuffer, video->framebuffer, sizeof(video->framebuffer));
}
