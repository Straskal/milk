#include "milk.h"
#include "milk_api.h"
#include <stdio.h>

#define COORD_2_FRAMEBUF_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)
#define HEX_2_COLOR(h) ((ColorRGB){((h >> 16)), ((h >> 8)), ((h)) })

Milk* milk_init()
{
	Milk* milk = (Milk*)calloc(1, sizeof(Milk));
	milk_load_code(milk);
	return milk;
}

void milk_free(Milk* milk)
{
	free(milk);
}

void milk_update(Milk* milk)
{
	milk_invoke_code_upd(&milk->code);
}

void milk_draw(Milk* milk)
{
	milk_invoke_code_draw(&milk->code);
}

void milk_clear(Video* vram, int hex)
{
	int i;
	ColorRGB color = HEX_2_COLOR(hex);
	for (i = 0; i < MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT; i++) 
	{
		vram->framebuffer[i] = color;
	}
}

static void __pixelset(Video* vram, ColorRGB* color, int x, int y)
{
	vram->framebuffer[COORD_2_FRAMEBUF_POS(x, y)] = *color;
}

void milk_pixelset(Video* vram, int hex, int x, int y)
{
	__pixelset(vram, hex, x, y);
}

void milk_rectfill(Video* vram, int idx, int x, int y, int w, int h)
{
	int i, j;
	ColorRGB color = HEX_2_COLOR(idx);
	for (i = x; i < x + w && i < MILK_FRAMEBUF_WIDTH; i++) 
	{
		for (j = y; j < y + h && j < MILK_FRAMEBUF_HEIGHT; j++)
		{
			__pixelset(vram, &color, i, j);
		}
	}
}

static void __horizontal_line(Video* vram, ColorRGB *color, int x, int y, int w) 
{
	int i;
	for (i = x; i <= x + w; i++)
	{
		if (i < MILK_FRAMEBUF_WIDTH)
			__pixelset(vram, color, i, y);
	}
}

static void __vertical_line(Video* vram, ColorRGB *color, int x, int y, int h)
{
	int i;
	for (i = y; i <= y + h; i++)
	{
		if (i < MILK_FRAMEBUF_WIDTH)
			__pixelset(vram, color, x, i);
	}
}

void milk_rect(Video* vram, int idx, int x, int y, int w, int h)
{
	ColorRGB color = HEX_2_COLOR(idx);
	__horizontal_line(vram, &color, x, y, w);
	__horizontal_line(vram, &color, x, y + h, w);
	__vertical_line(vram, &color, x, y, h);
	__vertical_line(vram, &color, x + w, y, h);
}
