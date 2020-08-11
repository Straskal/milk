#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdint.h>
#include <stdlib.h>

#include "bitmap.h"

#define FRAMERATE 50
#define FRAMEBUFFER_WIDTH 384
#define FRAMEBUFFER_HEIGHT 216
#define SPRITE_SIZE 8
#define FONT_SPRITE_SPACING 6

typedef struct
{
  int top;
  int left;
  int bottom;
  int right;
} Rect;

typedef struct
{
  uint32_t framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
  uint32_t colorKey;
  Rect clipRect;
} Video;

void initializeVideo(Video *video);
void resetDrawState(Video *video);
void setClip(Video *video, int x, int y, int w, int h);
void clearFramebuffer(Video *video, uint32_t color);
void drawPixel(Video *video, int x, int y, uint32_t color);
void drawLine(Video *video, int x0, int y0, int x1, int y1, uint32_t color);
void drawRect(Video *video, int x, int y, int w, int h, uint32_t color);
void drawFilledRect(Video *video, int x, int y, int w, int h, uint32_t color);
void drawSprite(Video *video, Bitmap *bmp, int index, int x, int y, int w, int h, float scale, uint8_t flip, uint32_t color);
void drawFont(Video *video, Bitmap *bmp, int x, int y, const char *text, int scale, uint32_t color);
void drawWrappedFont(Video *video, Bitmap *bmp, int x, int y, int w, const char *text, int scale, uint32_t color);
int getFontWidth(const char *text);
void flipFramebuffer(Video *video, uint32_t *frontBuffer);

#endif
