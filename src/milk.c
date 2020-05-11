#include "milk.h"
#include "milk_api.h"
#include "milk_bmp.h"
#include <memory.h>
#include <stdio.h>

#define COORD_2_FRAMEBUF_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)
#define HEX_2_COLOR(h) ((ColorRGB){((h >> 16)), ((h >> 8)), ((h)) })

Milk *milkInit()
{
	Milk *milk = (Milk *)calloc(1, sizeof(Milk));
	milkLoadSpritesheet(milk, MILK_SPR_FILENAME);
	milkLoadScripts(milk);
	return milk;
}

void milkFree(Milk *milk)
{
	free(milk);
}

void milkUpdate(Milk *milk)
{
	milkInvokeUpdate(&milk->code);
}

static void _resetDrawState(Video *video)
{
	video->colorKey = HEX_2_COLOR(0x000000);
}

void milkDraw(Milk *milk)
{
	_resetDrawState(&milk->video);
	milkInvokeDraw(&milk->code);
}

void milkClear(Video *video, int hex)
{
	int i;
	ColorRGB color = HEX_2_COLOR(hex);

	for (i = 0; i < MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT; i++)
	{
		video->framebuffer[i] = color;
	}
}

static void _framebufferCullXY(int *x, int *y)
{
	if (x != NULL)
	{
		if (x < 0)
			x = 0;
		else if (x > MILK_FRAMEBUF_WIDTH)
			x = MILK_FRAMEBUF_WIDTH;
	}
	if (y != NULL)
	{
		if (y < 0)
			y = 0;
		else if (y > MILK_FRAMEBUF_HEIGHT)
			y = MILK_FRAMEBUF_HEIGHT;
	}
}

static void _framebufferCullXYWH(int *x, int *y, int *w, int *h)
{
	_framebufferCullXY(x, y);

	int right = *x + *w;
	int bottom = *y + *h;
	int clampedRight = right;
	int clampedBottom = bottom; /* ouch! */

	_framebufferCullXY(&clampedRight, &clampedBottom);

	if (clampedRight != right)
		*w -= (right - clampedRight);
	if (clampedBottom != bottom)
		*h -= (bottom - clampedBottom);
}

static int _colorEqual(ColorRGB *col1, ColorRGB *col2)
{
	return col1->r == col2->r && col1->g == col2->g && col1->b == col2->b;
}

static void _pixelSet(Video *video, ColorRGB *color, int x, int y)
{
	video->framebuffer[COORD_2_FRAMEBUF_POS(x, y)] = *color;
}

void milkPixelSet(Video *video, int hex, int x, int y)
{
	_pixelSet(video, hex, x, y);
}

void milkRectFill(Video *video, int hex, int x, int y, int w, int h)
{
	int i = x, j = y;
	_framebufferCullXYWH(&i, &j, &w, &h);

	ColorRGB color = HEX_2_COLOR(hex);

	for (i = x; i < x + w; i++)
	{
		for (j = y; j < y + h; j++)
		{
			if (i < MILK_FRAMEBUF_WIDTH && j < MILK_FRAMEBUF_HEIGHT)
				_pixelSet(video, &color, i, j);
		}
	}
}

static void _horizontalLine(Video *video, ColorRGB *color, int x, int y, int w)
{
	int i;
	for (i = x; i <= x + w; i++)
	{
		if (i < MILK_FRAMEBUF_WIDTH)
			_pixelSet(video, color, i, y);
	}
}

static void _verticalLine(Video *video, ColorRGB *color, int x, int y, int h)
{
	int i;
	for (i = y; i <= y + h; i++)
	{
		if (i < MILK_FRAMEBUF_WIDTH)
			_pixelSet(video, color, x, i);
	}
}

void milkRect(Video *vram, int hex, int x, int y, int w, int h)
{
	ColorRGB color = HEX_2_COLOR(hex);

	_framebufferCullXYWH(&x, &y, &w, &h);
	_horizontalLine(vram, &color, x, y, w);
	_horizontalLine(vram, &color, x, y + h, w);
	_verticalLine(vram, &color, x, y, h);
	_verticalLine(vram, &color, x + w, y, h);
}

static void _blit(Video *video, ColorRGB* pixels, int x, int y)
{
	int framebufx, framebufy;
	int spritex, spritey;

	for (framebufx = x, spritex = 0; framebufx < x + MILK_SPR_SQRSIZE; framebufx++, spritex++)
	{
		for (framebufy = y, spritey = 0; framebufy < y + MILK_SPR_SQRSIZE; framebufy++, spritey++)
		{
			if (framebufx < MILK_FRAMEBUF_WIDTH && framebufy < MILK_FRAMEBUF_HEIGHT)
			{
				ColorRGB *col = &pixels[MILK_SPRSHEET_SQRSIZE * spritey + spritex];

				if (!_colorEqual(col, &video->colorKey))
					_pixelSet(video, col, framebufx, framebufy);
			}
		}
	}
}

void milkSprite(Video *video, int idx, int x, int y)
{
	if (idx < 0)
		return;
	if (idx > MILK_SPRSHEET_SQRSIZE)
		return;

	_blit(video, &video->spritesheet[idx * MILK_SPR_SQRSIZE], x, y);
}

int milkButton(Input *input, ButtonState button)
{
	return (input->gamepad.buttonState & button) == button;
}
