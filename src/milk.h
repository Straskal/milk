#ifndef __MILK_H__
#define __MILK_H__

#include <stdint.h>

#define MILK_MEM 1024
#define MILK_FRAMERATE (1000 / 30)
#define MILK_WINDOW_WIDTH 512
#define MILK_WINDOW_HEIGHT 512
#define MILK_FRAMEBUF_WIDTH 256
#define MILK_FRAMEBUF_HEIGHT 224
#define MILK_FRAMEBUF_BPP 4
#define MILK_FRAMEBUF_PITCH (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_BPP)
#define MILK_FRAMEBUF_SIZE (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT)
#define MILK_PALETTE_SIZE 16
#define MILK_VRAM_BYTES ((MILK_PALETTE_SIZE * sizeof(ColorRGB)) + MILK_FRAMEBUF_SIZE * MILK_FRAMEBUF_BPP)
#define MILK_MEMORY_BYTES (MILK_VRAM_BYTES)
#define MILK_BOOL int
#define MILK_TRUE 1
#define MILK_FALSE 0

typedef struct ColorRGB
{
	char r;
	char g;
	char b;
} ColorRGB;

typedef struct Video
{
	ColorRGB framebuffer[MILK_FRAMEBUF_SIZE];
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
	void* state;
} Code;

typedef struct MilkMachine
{
	Input input;
	Video video;
	Code code;
} Milk;

Milk* milk_init();
void milk_free(Milk* milk);
void milk_update(Milk* milk);
void milk_draw(Milk* milk);
void milk_clear(Video* vram, int idx);
void milk_pixelset(Video* vram, int hex, int x, int y);
void milk_rectfill(Video* vram, int hex, int x, int y, int w, int h);
void milk_rect(Video* vram, int hex, int x, int y, int w, int h);

#endif
