#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdint.h>
#include <stdlib.h>

#define FRAMERATE 50
#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 224
#define WINDOW_WIDTH (FRAMEBUFFER_WIDTH * 3)
#define WINDOW_HEIGHT (FRAMEBUFFER_HEIGHT * 3)
#define FONTS_MAX 2
#define FONT_WIDTH 96
#define FONT_HEIGHT 64
#define FONT_CHAR_WIDTH 8
#define FONT_CHAR_HEIGHT 8
#define FONT_CHAR_SPACING 6

typedef enum {
  Additive,
  Average,
  Solid,
} ColorMode;

typedef struct {
  int top;
  int bottom;
  int left;
  int right;
} Rect;

typedef struct {
  uint32_t *pixels;
  int width;
  int height;
} Bitmap;

typedef struct {
  uint32_t framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
  uint32_t embeddedFont[FONT_WIDTH * FONT_HEIGHT];
  uint32_t colorKey;
  Rect clipRect;
} Video;

void initializeVideo(Video *video);
void disableVideo(Video *video);
Bitmap *loadBitmap(const char *filePath);
void freeBitmap(Bitmap *bitmap);
void resetDrawState(Video *video);
void clip(Video *video, int x, int y, int w, int h);
void clearFramebuffer(Video *video, uint32_t color);
void pixel(Video *video, int x, int y, uint32_t color);
void line(Video *video, int x0, int y0, int x1, int y1, uint32_t color);
void rect(Video *video, int x, int y, int w, int h, uint32_t color);
void rectFill(Video *video, int x, int y, int w, int h, uint32_t color);
void sprite(Video *video, Bitmap *bmp, int index, int x, int y, int w, int h, float scale, uint8_t flip, uint32_t color, ColorMode mode);
void font(Video *video, Bitmap *bmp, int x, int y, const char *str, int scale, uint32_t color);
int fontWidth(const char *text);

#endif
