/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software andassociated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, andto permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice andthis permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "milk.h"
#include "milk_api.h"
#include "milk_bmp.h"

#define FRAMEBUFFER_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)
#define NOT_CLIPPED(clip, x, y) (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

static void _initVideo(Video *video)
{
	milkLoadSpritesheet(video->spritesheet, MILK_SPRSHEET_FILENAME);
	milkLoadFont(video->font, MILK_FONT_FILENAME);
}

Milk *milkInit()
{
	Milk *milk = (Milk *)calloc(1, sizeof(Milk));
	milkOpenAudio(&milk->audio);
	_initVideo(&milk->video);

	milkLoadScripts(milk);
	return milk;
}

void milkFree(Milk *milk)
{
	milkUnloadScripts(milk);
	milkCloseAudio(&milk->audio);
	free(milk);
}

void milkUpdate(Milk *milk)
{
	milkInvokeUpdate(&milk->code);
}

static void _resetDrawState(Video *video)
{
	video->colorKey = 0;
	video->clipRect.top = 0;
	video->clipRect.bottom = MILK_FRAMEBUF_HEIGHT;
	video->clipRect.right = MILK_FRAMEBUF_WIDTH;
	video->clipRect.left = 0;
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

int milkButtonPressed(Input *input, uint8_t button)
{
	return (input->gamepad.buttonState & button) == button && (input->gamepad.previousButtonState & button) != button;
}

void milkClipRect(Video *video, int x, int y, int w, int h)
{
	int right = x + w;
	int bottom = y + h;

	if (x < 0) x = 0;
	else if (x > MILK_FRAMEBUF_WIDTH) x = MILK_FRAMEBUF_WIDTH;
	if (y < 0) y = 0;
	else if (y > MILK_FRAMEBUF_HEIGHT) y = MILK_FRAMEBUF_HEIGHT;
	if (right < 0) right = 0;
	else if (right > MILK_FRAMEBUF_WIDTH) right = MILK_FRAMEBUF_WIDTH;
	if (bottom < 0) bottom = 0;
	else if (bottom > MILK_FRAMEBUF_HEIGHT) bottom = MILK_FRAMEBUF_HEIGHT;

	video->clipRect.left = x;
	video->clipRect.right = right;
	video->clipRect.top = y;
	video->clipRect.bottom = bottom;
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
	if (NOT_CLIPPED(video->clipRect, x, y))
		video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color)
{
	for (int j = y; j < y + h; j++)
	{
		for (int i = x; i < x + w; i++)
		{
			milkPixelSet(video, i, j, color);
		}
	}
}

static void _horizontalLine(Video *video, int x, int y, int w, Color32 color)
{
	for (int i = x; i <= x + w; i++)
	{
		milkPixelSet(video, i, y, color);
	}
}

static void _verticalLine(Video *video, int x, int y, int h, Color32 color)
{
	for (int i = y; i <= y + h; i++)
	{
		milkPixelSet(video, x, i, color);
	}
}

void milkRect(Video *video, int x, int y, int w, int h, Color32 color)
{
	_horizontalLine(video, x, y, w, color);
	_horizontalLine(video, x, y + h, w, color);
	_verticalLine(video, x, y, h, color);
	_verticalLine(video, x + w, y, h, color);
}

static void _blitRect(Video *video, Color32 *pixels, int x, int y, int w, int h, int pitch, float scale, int xflip)
{
	int xPixel;
	int yPixel;
	int xFramebuffer;
	int yFramebuffer;

	if (scale == 1.0f)
	{
		for (yFramebuffer = y, yPixel = 0; yFramebuffer < y + h; yFramebuffer++, yPixel++)
		{
			for (xFramebuffer = x, xPixel = 0; xFramebuffer < x + w; xFramebuffer++, xPixel++)
			{
				Color32 col = pixels[yPixel * pitch + xPixel];

				if (col != video->colorKey)
					milkPixelSet(video, xFramebuffer, yFramebuffer, col);
			}
		}
	}
	else
	{
		float scaledWidth = w * scale;
		float scaledHeight = h * scale;
		int xRatio = (int)((w << 16) / scaledWidth) + 1;
		int yRatio = (int)((h << 16) / scaledHeight) + 1;

		for (yFramebuffer = y, yPixel = 0; yFramebuffer < y + scaledHeight; yFramebuffer++, yPixel++)
		{
			for (xFramebuffer = x, xPixel = 0; xFramebuffer < x + scaledWidth; xFramebuffer++, xPixel++)
			{
				int xNearest = (xPixel * xRatio) >> 16;
				int yNearest = (yPixel * yRatio) >> 16;

				Color32 col = pixels[yNearest * pitch + xNearest];

				if (col != video->colorKey)
					milkPixelSet(video, xFramebuffer, yFramebuffer, col);
			}
		}
	}
}

void milkSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int xflip)
{
	if (idx < 0 || MILK_SPRSHEET_SQRSIZE < idx)
		return;

	int numColumns = MILK_SPRSHEET_SQRSIZE / MILK_SPRSHEET_SPR_SQRSIZE;
	int row = (int)floor(idx / numColumns);
	int col = (int)floor(idx % numColumns);
	Color32 *pixels = &video->spritesheet[row * MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SPR_SQRSIZE + col * MILK_SPRSHEET_SPR_SQRSIZE];

	_blitRect(video, pixels, x, y, MILK_SPRSHEET_SPR_SQRSIZE * w, MILK_SPRSHEET_SPR_SQRSIZE * h, MILK_SPRSHEET_SQRSIZE, scale, xflip);
}

static int _isAscii(char character)
{
	return (character & 0xff80) == 0;
}

static void _drawCharacter(Video *video, int x, int y, char character, float scale)
{
	if (!_isAscii(character))
		character = '?';

	/* bitmap font starts at ASCII character 32 (SPACE) */
	int numColumns = MILK_FONT_WIDTH / MILK_CHAR_SQRSIZE;
	int row = (int)floor((character - 32) / numColumns);
	int col = (int)floor((character - 32) % numColumns);
	Color32 *pixels = &video->font[(row * MILK_FONT_WIDTH * MILK_CHAR_SQRSIZE + col * MILK_CHAR_SQRSIZE)];

	_blitRect(video, pixels, x, y, MILK_CHAR_SQRSIZE, MILK_CHAR_SQRSIZE, MILK_FONT_WIDTH, scale, 0);
}

int _isNewline(const char *characters)
{
	return *characters == '\n';
}

void milkSpriteFont(Video *video, int x, int y, const char *str, float scale)
{
	int xCurrent = x;
	int yCurrent = y;

	while (*str)
	{
		while (_isNewline(str))
		{
			xCurrent = x;
			yCurrent += MILK_CHAR_SQRSIZE * scale;
			str++;
		}

		_drawCharacter(video, xCurrent, yCurrent, *(str++), scale);
		xCurrent += MILK_CHAR_SQRSIZE * scale;
	}
}
