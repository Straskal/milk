#include "milk.h"
#include "milk_api.h"
#include "milk_bmp.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>

#define FRAMEBUFFER_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)
#define FRAMEBUFFER_CAN_SET(x, y) (0 < x && x < MILK_FRAMEBUF_WIDTH && 0 < y && y < MILK_FRAMEBUF_HEIGHT)

Milk *milkInit()
{
	Milk *milk = (Milk *)calloc(1, sizeof(Milk));
	milkLoadSpritesheet(&milk->video.spritesheet, MILK_SPRSHEET_FILENAME);
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

static void _resetDrawState(Video *video)
{
	video->colorKey = 0;
}

void milkDraw(Milk *milk)
{
	_resetDrawState(&milk->video);
	milkInvokeDraw(&milk->code);
}

int milkButton(Input *input, uint8_t button)
{
	return (input->gamepad.buttonState & button) == button;
}

void milkClear(Video *video, Color32 color)
{
	for (int i = 0; i < MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT; i++)
	{
		video->framebuffer[i] = color;
	}
}

void milkPixelSet(Video *video, int x, int y, Color32 color)
{
	if (FRAMEBUFFER_CAN_SET(x, y))
		video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color)
{
	int right = x + w;
	int bottom = y + h;

	for (int j = y; j < bottom; j++)
	{
		for (int i = x; i < right; i++)
		{
			milkPixelSet(video, i, j, color);
		}
	}
}

static void _horizontalLine(Video *video, Color32 color, int x, int y, int w)
{
	for (int i = x; i <= x + w; i++)
	{
		milkPixelSet(video, i, y, color);
	}
}

static void _verticalLine(Video *video, Color32 color, int x, int y, int h)
{
	for (int i = y; i <= y + h; i++)
	{
		milkPixelSet(video, x, i, color);
	}
}

void milkRect(Video *vram, int x, int y, int w, int h, Color32 color)
{
	int right = x + w;
	int bottom = y + h;

	_horizontalLine(vram, color, x, y, w);
	_horizontalLine(vram, color, x, bottom, w);
	_verticalLine(vram, color, x, y, h);
	_verticalLine(vram, color, right, y, h);
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
				milkPixelSet(video, framebufferX, framebufferY, col);
		}
	}
}

static void _blitRectNearestNeighbor(Video *video, Color32 *pixels, int x, int y, int w, int h, int pitch, float scale)
{
	int framebufferX, framebufferY, pixelX, pixelY, nnPixelX, nnPixelY;
	float scaledWidth = w * scale;
	float scaledHeight = h * scale;
	int xRatio = (int)((w << 16) / scaledWidth) + 1;
	int yRatio = (int)((h << 16) / scaledHeight) + 1;

	for (framebufferY = y, pixelX = 0; framebufferY < y + scaledWidth; pixelX++, framebufferY++)
	{
		for (framebufferX = x, pixelY = 0; framebufferX < x + scaledWidth; pixelY++, framebufferX++)
		{
			nnPixelX = (pixelY * xRatio) >> 16;
			nnPixelY = (pixelX * yRatio) >> 16;

			Color32 col = pixels[nnPixelY * pitch + nnPixelX];

			if (col == video->colorKey)
				milkPixelSet(video, framebufferX, framebufferY, col);
		}
	}
}

void milkSprite(Video *video, int idx, int x, int y)
{
	milkScaledSprite(video, idx, x, y, 1.0f);
}

void milkScaledSprite(Video *video, int idx, int x, int y, float scale)
{
	if (idx < 0 || MILK_SPRSHEET_SQRSIZE < idx)
		return;

	int row = floor(idx / 16);
	int col = floor(idx % 16);
	Color32 *pixels = &video->spritesheet[row * MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SPR_SQRSIZE + col * MILK_SPRSHEET_SPR_SQRSIZE];
	
	if (scale == 1.0f)
		_blitRect(video, pixels, x, y, MILK_SPRSHEET_SPR_SQRSIZE, MILK_SPRSHEET_SPR_SQRSIZE, MILK_SPRSHEET_SQRSIZE);
	else
		_blitRectNearestNeighbor(video, pixels, x, y, MILK_SPRSHEET_SPR_SQRSIZE, MILK_SPRSHEET_SPR_SQRSIZE, MILK_SPRSHEET_SQRSIZE, scale);
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
	Color32 *pixels = &video->font[(row * MILK_FONT_WIDTH * MILK_CHAR_SQRSIZE + col * MILK_CHAR_SQRSIZE)];

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
