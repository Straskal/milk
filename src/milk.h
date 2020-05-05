#ifndef __MILK_H__
#define __MILK_H__

#include <stdint.h>

#define MILK_MEM 1024
#define MILK_FRAMERATE (1000 / 30)

#define MILK_WINDOW_WIDTH 512
#define MILK_WINDOW_HEIGHT 512

#define MILK_FRAMEBUF_WIDTH 128
#define MILK_FRAMEBUF_HEIGHT 128
#define MILK_FRAMEBUF_BPP 4
#define MILK_FRAMEBUF_PITCH (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_BPP)
#define MILK_FRAMEBUF_BYTES (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT)

#define MILK_PALETTE_SIZE 16

#define MILK_VRAM_BYTES ((MILK_PALETTE_SIZE * sizeof(ColorRGB)) + MILK_FRAMEBUF_BYTES * MILK_FRAMEBUF_BPP)
#define MILK_MEMORY_BYTES (MILK_VRAM_BYTES)

#define MILK_TRUE 1
#define MILK_FALSE 0

typedef union
{
	struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	uint8_t data[3];
} ColorRGB;

typedef union
{
	struct
	{
		ColorRGB palette[MILK_PALETTE_SIZE];
		uint32_t framebuffer[MILK_FRAMEBUF_BYTES];
	};

	uint8_t data[MILK_VRAM_BYTES];
} VRAM;

typedef union
{
	struct
	{
		VRAM vram;
	};

	uint8_t data[MILK_MEMORY_BYTES];
} Memory;

typedef struct
{
	uint32_t msx;
	uint32_t msy;
} Input;

typedef struct
{
	void* state;
} Code;

typedef struct
{
	Memory memory;
	Input input;
	Code code;

	void(*update)();
	void(*draw)();
} MilkMachine;

MilkMachine* MilkInit();
void MilkFree(MilkMachine* milk);
void MilkUpdate(MilkMachine* milk);
void MilkDraw(MilkMachine* milk);
void MilkClear(VRAM* vram, int idx);
void MilkDrawPixel(VRAM* vram, char idx, int x, int y);
void MilkDrawRect(VRAM* vram, char idx, int x, int y, int w, int h);
void MilkDrawRectLines(VRAM* vram, char idx, int x, int y, int w, int h);

#endif