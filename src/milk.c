#include "milk.h"
#include "milk_api.h"
#include "milk_bmp.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>

#define FRAMEBUFFER_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)
#define HEX_2_COLOR(h) ((ColorRGB){((h >> 16)), ((h >> 8)), ((h)) })


Milk *milkInit()
{
	Milk *milk = (Milk *)calloc(1, sizeof(Milk));
	milkLoadSpritesheet(&milk->video.spritesheet, MILK_SPR_FILENAME);
	milkLoadFont(&milk->video.font, MILK_FONT_FILENAME);
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

int milkButton(Input *input, uint8_t button)
{
	return (input->gamepad.buttonState & button) == button;
}

static void _resetDrawState(Video *video)
{
	video->colorKey = 0;
}

void milkDraw(Milk *milk)
{
	_resetDrawState(&milk->video);
	milkInvokeDraw(&milk->code);
}

void milkClear(Video *video, Color32 hex)
{
	for (int i = 0; i < MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT; i++)
	{
		video->framebuffer[i] = hex;
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
		*w -= right - clampedRight;

	if (clampedBottom != bottom)
		*h -= bottom - clampedBottom;
}

void milkPixelSet(Video *video, int x, int y, Color32 hex)
{
	video->framebuffer[FRAMEBUFFER_POS(x, y)] = hex;
}

void milkRectFill(Video *video, int x, int y, int w, int h, Color32 hex)
{
	int right = x + w;
	int bottom = y + h;

	_framebufferCullXY(&x, &y);
	_framebufferCullXY(&right, &bottom);

	for (int j = y; j < bottom; j++)
	{
		for (int i = x; i < right; i++)
		{
			video->framebuffer[FRAMEBUFFER_POS(i, j)] = hex;
		}
	}
}

static void _horizontalLine(Video *video, Color32 color, int x, int y, int w)
{
	for (int i = x; i <= x + w; i++)
	{
		video->framebuffer[FRAMEBUFFER_POS(i, y)] = color;
	}
}

static void _verticalLine(Video *video, Color32 color, int x, int y, int h)
{
	for (int i = y; i <= y + h; i++)
	{
		video->framebuffer[FRAMEBUFFER_POS(x, i)] = color;
	}
}

void milkRect(Video *vram, int x, int y, int w, int h, Color32 hex)
{
	int right = x + w;
	int bottom = y + h;

	_framebufferCullXY(&x, &y);
	_framebufferCullXY(&right, &bottom);

	_horizontalLine(vram, hex, x, y, w);
	_horizontalLine(vram, hex, x, bottom, w);
	_verticalLine(vram, hex, x, y, h);
	_verticalLine(vram, hex, right, y, h);
}

static void _blitRect(Video *video, Color32 *pixels, int x, int y, int w, int h, int pitch)
{
	int framebufferX, framebufferY, pixelX, pixelY;

	for (framebufferY = y, pixelY = 0; framebufferY < y + h; framebufferY++, pixelY++)
	{
		for (framebufferX = x, pixelX = 0; framebufferX < x + w; framebufferX++, pixelX++)
		{
			Color32 col = pixels[pixelY * pitch + pixelX];

			if (col != video->colorKey)
				video->framebuffer[FRAMEBUFFER_POS(framebufferX, framebufferY)] = col;
		}
	}
}

static void _blitRectNearestNeighbor(Video *video, Color32 *pixels, int x, int y, int w, int h, int pitch, float scale)
{
	int framebufferX, framebufferY, pixelX, pixelY, pixelX2, pixelY2;
	float scaledWidth = w * scale;
	float scaledHeight = h * scale;
	int xRatio = (int)((w << 16) / scaledWidth) + 1;
	int yRatio = (int)((h << 16) / scaledHeight) + 1;

	for (framebufferY = y, pixelX = 0; framebufferY < y + scaledWidth; pixelX++, framebufferY++)
	{
		for (framebufferX = x, pixelY = 0; framebufferX < x + scaledWidth; pixelY++, framebufferX++)
		{
			pixelX2 = (pixelY * xRatio) >> 16;
			pixelY2 = (pixelX * yRatio) >> 16;

			Color32 col = pixels[pixelY2 * pitch + pixelX2];

			if (col == video->colorKey)
				video->framebuffer[FRAMEBUFFER_POS(framebufferX, framebufferY)] = col;
		}
	}
}

void milkSprite(Video *video, int idx, int x, int y)
{
	milkScaledSprite(video, idx, x, y, 1.0f);
}

void milkScaledSprite(Video *video, int idx, int x, int y, float scale)
{
	if (idx < 0)
		return;

	if (idx > MILK_SPRSHEET_SQRSIZE)
		return;

	int row = floor(idx / 16);
	int col = floor(idx % 16);
	Color32 *pixels = &video->spritesheet[row * MILK_SPRSHEET_SQRSIZE * MILK_SPR_SQRSIZE + col * MILK_SPR_SQRSIZE];
	
	if (scale == 1.0f)
		_blitRect(video, pixels, x, y, MILK_SPR_SQRSIZE, MILK_SPR_SQRSIZE, MILK_SPRSHEET_SQRSIZE);
	else
		_blitRectNearestNeighbor(video, pixels, x, y, MILK_SPR_SQRSIZE, MILK_SPR_SQRSIZE, MILK_SPRSHEET_SQRSIZE, scale);
}

static int _isAscii(char character)
{
	return (character & 0xff80) == 0;
}

static void _drawCharacter(Video *video, char character, int x, int y)
{
	if (!_isAscii(character))
		character = '?';

	/* bitmap font starts at ASCII character 32 (SPACE) */
	int row = floor((character - 32) / 16);
	int col = floor((character - 32) % 16);
	Color32 *pixels = &video->font[(row * MILK_FONT_PITCH + col * MILK_CHAR_SQRSIZE)];

	_blitRect(video, pixels, x, y, MILK_CHAR_SQRSIZE, MILK_CHAR_SQRSIZE, MILK_FONT_WIDTH);
}

void milkSpriteFont(Video *video, int x, int y, const char *str)
{
	char *currentChar = str;

	while (*currentChar)
	{
		_drawCharacter(video, *(currentChar++), x, y);
		x += MILK_CHAR_SQRSIZE;
	}
}
