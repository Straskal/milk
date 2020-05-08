#include "sprite.h"
#include <stdio.h>
#include <math.h>

#define PALETTE_SIZE 64
#define PALETTE_COLOR_SIZE (PALETTE_SIZE / 4)
#define PALETTE_START_XPOS (MILK_FRAMEBUF_WIDTH - PALETTE_SIZE - 16)
#define PALETTE_START_YPOS (16)

static void DrawPalette(MilkMachine* milk)
{
	int i;
	int curx = PALETTE_START_XPOS;
	int cury = PALETTE_START_YPOS;
	ColorRGB* palette = milk->memory.vram.palette;
	for (i = 0; i < MILK_PALETTE_SIZE; i++)
	{
		MilkDrawRect(&milk->memory.vram, i, curx, cury, PALETTE_COLOR_SIZE, PALETTE_COLOR_SIZE);
		if ((i + 1) % 4 == 0)
		{
			curx = PALETTE_START_XPOS;
			cury += PALETTE_COLOR_SIZE;
		}
		else
		{
			curx += PALETTE_COLOR_SIZE;
		}
	}
	MilkDrawRectLines(&milk->memory.vram, 7, PALETTE_START_XPOS - 1, PALETTE_START_YPOS - 1, PALETTE_SIZE + 1, PALETTE_SIZE + 1);
}

static void DetectPaletteSelection(MilkMachine* milk) 
{
	Input input = milk->input;
	if (input.msdown && input.msdownp == 0) 
	{
		uint32_t msx = input.msx;
		uint32_t msy = input.msy;

		if (msx > PALETTE_START_XPOS && msx < PALETTE_START_XPOS + PALETTE_SIZE && msy > PALETTE_START_YPOS && msy < PALETTE_START_YPOS + PALETTE_SIZE)
		{
			
		}
	}
}

void SpriteEditorUpdate(MilkMachine* milk)
{
}

void SpriteEditorDraw(MilkMachine* milk)
{
	MilkClear(milk, 1);
	DrawPalette(milk);
	DetectPaletteSelection(milk);
}
