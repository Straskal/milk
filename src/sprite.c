#include "sprite.h"

#define PALETTE_WIDTH 32
#define PALETTE_COLOR_SIZE (PALETTE_WIDTH / 4)
#define PALETTE_START_XPOS (MILK_FRAMEBUF_WIDTH - PALETTE_WIDTH - 12)

static void DrawPalette(MilkMachine* milk)
{
	int i;
	int curx = PALETTE_START_XPOS;
	int cury = 16;
	ColorRGB* palette = &milk->memory.vram.palette;

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

	MilkDrawRectLines(&milk->memory.vram, 7, PALETTE_START_XPOS - 1, 15, 33, 33);
}

void SpriteEditorUpdate(MilkMachine* milk)
{
}

void SpriteEditorDraw(MilkMachine* milk)
{
	MilkClear(milk, 1);
	DrawPalette(milk);
}
