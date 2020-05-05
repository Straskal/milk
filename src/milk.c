#include "milk.h"
#include "sprite.h"
#include <stdio.h>

#define PACKED_COLOR(col) (col.r << 24 | (col.g << 16) | (col.b << 8) | 0x00)
#define HEX_2_COLOR(h) ((ColorRGB){((h >> 16)), ((h >> 8)), ((h)) })
#define COORD_2_FRAMEBUF_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)

static void InitializePalette(VRAM* vram)
{
	vram->palette[0] = HEX_2_COLOR(0x000000);
	vram->palette[1] = HEX_2_COLOR(0x1d2b53);
	vram->palette[2] = HEX_2_COLOR(0x7e2553);
	vram->palette[3] = HEX_2_COLOR(0x008751);
	vram->palette[4] = HEX_2_COLOR(0xab5236);
	vram->palette[5] = HEX_2_COLOR(0x1d2b53);
	vram->palette[6] = HEX_2_COLOR(0x1d2b53);
	vram->palette[7] = HEX_2_COLOR(0x1d2b53);
	vram->palette[8] = HEX_2_COLOR(0x1d2b53);
	vram->palette[9] = HEX_2_COLOR(0x1d2b53);
	vram->palette[10] = HEX_2_COLOR(0x1d2b53);
	vram->palette[11] = HEX_2_COLOR(0x1d2b53);
	vram->palette[12] = HEX_2_COLOR(0x1d2b53);
	vram->palette[13] = HEX_2_COLOR(0x1d2b53);
	vram->palette[14] = HEX_2_COLOR(0x1d2b53);
	vram->palette[15] = HEX_2_COLOR(0x1d2b53);
}

static void GameUpdate(MilkMachine* milk) 
{
	MilkInvokeCodeUpdate(&milk->code);
}

/*=============================================================*/

MilkMachine* MilkInit()
{
	MilkMachine* milk = (MilkMachine*)calloc(1, sizeof(MilkMachine));
	milk->update = SpriteEditorDraw;
	InitializePalette(&milk->memory.vram);
	MilkLoadCode(milk);
	return milk;
}

/*=============================================================*/

void MilkFree(MilkMachine* milk)
{
	free(milk);
}

/*=============================================================*/

void MilkUpdate(MilkMachine* milk)
{
	milk->update(milk);
}

/*=============================================================*/

void MilkDraw(MilkMachine* milk)
{
	//MilkInvokeCodeDraw(&milk->code);
}

/*=============================================================*/

void MilkClear(VRAM* vram, int idx)
{
	uint32_t packed = PACKED_COLOR(vram->palette[idx]);
	int i;
	for (i = 0; i < MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT; i++) 
	{
		vram->framebuffer[i] = packed;
	}
}

/*=============================================================*/

void MilkDrawPixel(VRAM* vram, char idx, int x, int y)
{
	vram->framebuffer[COORD_2_FRAMEBUF_POS(x, y)] = PACKED_COLOR(vram->palette[idx]);
}

/*=============================================================*/

void MilkDrawRect(VRAM* vram, char idx, int x, int y, int w, int h)
{
	uint32_t packed = PACKED_COLOR(vram->palette[idx]);
	int i, j;
	for (i = x; i < x + w && i < MILK_FRAMEBUF_WIDTH; i++) 
	{
		for (j = y; j < y + h && j < MILK_FRAMEBUF_HEIGHT; j++)
		{
			vram->framebuffer[COORD_2_FRAMEBUF_POS(i, j)] = packed;
		}
	}
}
