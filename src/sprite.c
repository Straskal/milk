#include "sprite.h"

static void DrawPalette(MilkMachine* milk)
{
	const int palettePixels = 32;
	const int colorPixels = palettePixels / 4;
	ColorRGB* palette = &milk->memory.vram.palette;

	int curx = 64;
	int cury = 16;
	int i;
	for (i = 0; i < 4; i++) 
	{
		MilkDrawRect(&milk->memory.vram, i, curx + (colorPixels * i), cury + (colorPixels * i), colorPixels, colorPixels);

		if (i % 4 == 0) 
		{
			curx = 0;
			cury++;
		}
		else 
		{
			curx++;
		}
	}
}

void SpriteEditorDraw(MilkMachine* milk)
{
	MilkClear(milk, 0);
	DrawPalette(milk);
}
