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
#define FONTS_MAX 2
#define FONT_WIDTH 96
#define FONT_HEIGHT 64
#define FONT_CHAR_WIDTH 8
#define FONT_CHAR_HEIGHT 8
#define FONT_CHAR_SPACING 6

typedef u32 Color32;

typedef enum
{
  None,
  Additive,
  Average,
} BlendMode;

typedef struct
{
  int top;
  int bottom;
  int left;
  int right;
} Rect;

typedef struct
{
  Color32 framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
  Color32 spriteSheet[SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE];
  Color32 fonts[FONTS_MAX][FONT_WIDTH * FONT_HEIGHT];
  Color32 embeddedFont[FONT_WIDTH * FONT_HEIGHT];
  Color32 colorKey;
  BlendMode blendMode;
  Rect clipRect;

  void (*loadBMP)(const char *, Color32 *, size_t);
} Video;

void initializeVideo(Video *video);
void disableVideo(Video *video);
void loadSpriteSheet(Video *video, const char *path);
void loadFont(Video *video, int id, const char *path);
void resetDrawState(Video *video);
void setClippingRect(Video *video, int x, int y, int w, int h);
void setBlendMode(Video *video, BlendMode mode);
void clearFramebuffer(Video *video, Color32 color);
void blitPixel(Video *video, int x, int y, Color32 color);
void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color);
void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color);
void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color);
void blitSprite(Video *video, int id, int x, int y, int w, int h, int scale, u8 flip);
int fontWidth(const char *text);
void blitFont(Video *video, int id, int x, int y, const char *str, int scale, Color32 color);

#endif
