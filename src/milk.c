#include "milk.h"
#include "milk_api.h"
#include <stdio.h>

#define COORD_2_FRAMEBUF_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)
#define HEX_2_COLOR(h) ((ColorRGB){((h >> 16)), ((h >> 8)), ((h)) })

Milk *milk_init()
{
	Milk *milk = (Milk *)calloc(1, sizeof(Milk));
	milk_load_code(milk);
	return milk;
}

void milk_free(Milk *milk)
{
	free(milk);
}

void milk_update(Milk *milk)
{
	milk_invoke_update(&milk->code);
}

void milk_draw(Milk *milk)
{
	milk_invoke_draw(&milk->code);
}

void milk_clear(Video *video, int hex)
{
	int i;
	ColorRGB color = HEX_2_COLOR(hex);
	for (i = 0; i < MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT; i++)
	{
		video->framebuffer[i] = color;
	}
}

static void __pixelset(Video *video, ColorRGB *color, int x, int y)
{
	video->framebuffer[COORD_2_FRAMEBUF_POS(x, y)] = *color;
}

void milk_pixelset(Video *video, int hex, int x, int y)
{
	__pixelset(video, hex, x, y);
}

void milk_rectfill(Video *video, int hex, int x, int y, int w, int h)
{
	int i, j;
	ColorRGB color = HEX_2_COLOR(hex);
	for (i = x; i < x + w; i++)
	{
		for (j = y; j < y + h; j++)
		{
			if (i < MILK_FRAMEBUF_WIDTH && j < MILK_FRAMEBUF_HEIGHT)
				__pixelset(video, &color, i, j);
		}
	}
}

static void __horizontal_line(Video *video, ColorRGB *color, int x, int y, int w)
{
	int i;
	for (i = x; i <= x + w; i++)
	{
		if (i < MILK_FRAMEBUF_WIDTH)
			__pixelset(video, color, i, y);
	}
}

static void __vertical_line(Video *video, ColorRGB *color, int x, int y, int h)
{
	int i;
	for (i = y; i <= y + h; i++)
	{
		if (i < MILK_FRAMEBUF_WIDTH)
			__pixelset(video, color, x, i);
	}
}

void milk_rect(Video *vram, int hex, int x, int y, int w, int h)
{
	ColorRGB color = HEX_2_COLOR(hex);
	__horizontal_line(vram, &color, x, y, w);
	__horizontal_line(vram, &color, x, y + h, w);
	__vertical_line(vram, &color, x, y, h);
	__vertical_line(vram, &color, x + w, y, h);
}
