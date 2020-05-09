#ifndef __MILK_H__
#define __MILK_H__

#include <stdint.h>

#define MILK_FRAMERATE (1000 / 30) /* Fixed 30 FPS */
#define MILK_WINDOW_WIDTH 512
#define MILK_WINDOW_HEIGHT 512
#define MILK_FRAMEBUF_WIDTH 256
#define MILK_FRAMEBUF_HEIGHT 224
#define MILK_FRAMEBUF_BPP 4
#define MILK_FRAMEBUF_PITCH (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_BPP)
#define MILK_FRAMEBUF_AREA (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT)
#define MILK_BOOL int
#define MILK_TRUE 1
#define MILK_FALSE 0

/* 3 channel color */
typedef struct ColorRGB
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ColorRGB;

typedef struct Video
{
	ColorRGB framebuffer[MILK_FRAMEBUF_AREA];
} Video;

typedef struct Input
{
	uint32_t msx;
	uint32_t msy;
	MILK_BOOL msdown;
	MILK_BOOL msdownp;
} Input;

typedef struct Code
{
	void *state;
} Code;

typedef struct Milk
{
	Input input;
	Video video;
	Code code;
} Milk;

Milk *milk_init();
void milk_free(Milk *milk);
void milk_update(Milk *milk);
void milk_draw(Milk *milk);
void milk_clear(Video *video, int idx);
void milk_pixelset(Video *video, int hex, int x, int y);
void milk_rectfill(Video *video, int hex, int x, int y, int w, int h);
void milk_rect(Video *video, int hex, int x, int y, int w, int h);

#endif
