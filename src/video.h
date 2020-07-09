#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdlib.h>

#include "common.h"

#define FRAMERATE 50
#define FRAMEBUFFER_WIDTH 256
#define FRAMEBUFFER_HEIGHT 224
#define WINDOW_WIDTH (FRAMEBUFFER_WIDTH * 3)
#define WINDOW_HEIGHT (FRAMEBUFFER_HEIGHT * 3)
#define SPRITE_SHEET_SQRSIZE 256
#define SPRITE_SQRSIZE 16
#define FONT_WIDTH 128
#define FONT_HEIGHT 48
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

typedef u32 Color32;

typedef struct rect
{
  int top;
  int bottom;
  int left;
  int right;
} Rect;

typedef struct video
{
  Color32 framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
  Color32 spriteSheet[SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE];
  Color32 font[FONT_WIDTH * FONT_HEIGHT];
  Color32 colorKey;
  Rect clipRect;

  void (*loadBMP)(const char *, Color32 *, size_t);
} Video;

void initializeVideo(Video *video);
void disableVideo(Video *video);
void loadSpriteSheet(Video *video, const char *path);
void loadFont(Video *video, const char *path);
void resetDrawState(Video *video);
void setClippingRect(Video *video, int x, int y, int w, int h);
void clearFramebuffer(Video *video, Color32 color);
void blitPixel(Video *video, int x, int y, Color32 color);
void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color);
void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color);
void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color);
void blitSprite(Video *video, int idx, int x, int y, int w, int h, int scale, u8 flip);
void blitSpriteFont(Video *video, const Color32 *pixels, int x, int y, const char *str, int scale, Color32 color);

#endif
