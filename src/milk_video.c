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

#include "milk_video.h"

#include <SDL.h>

#define FRAMEBUFFER_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x)
#define NOT_CLIPPED(clip, x, y) (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

#define FLIP_X 1
#define FLIP_Y 2

static void _loadBmp(Color32 *dest, char *filename)
{
	SDL_Surface *bmp = SDL_LoadBMP(filename);
	int bpp = bmp->format->BytesPerPixel;
	int width = bmp->w;
	int height = bmp->h;
	int len = width * height * bpp;
	uint8_t *bmpPixels = (Uint8 *)bmp->pixels;
	uint8_t *itr = bmpPixels;
	uint8_t *end = &bmpPixels[len];
	Color32 *pixelitr = dest;

	while (itr != end)
	{
		int b = *(itr++);
		int g = *(itr++);
		int r = *(itr++);

		*(pixelitr++) = (r << 16) | (g << 8) | (b);
	}

	SDL_FreeSurface(bmp);
}

void milkOpenVideo(Video *video)
{
	_loadBmp(&video->spritesheet, MILK_SPRSHEET_FILENAME);
	_loadBmp(&video->font, MILK_FONT_FILENAME);
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

static void _blitRect(Video *video, Color32 *pixels, int x, int y, int w, int h, int pitch, float scale, int flip)
{
	int xPixel, yPixel; /* Pixel position to draw. */
	int xFramebuffer, yFramebuffer; /* Position to draw on to the frame buffer. */
	int xPixelStart, xDirection; /* The x pixel to start drawing from and the direction to iterate */
	int yPixelStart, yDirection; /* The y pixel to start drawing from and the direction to iterate */
	int width = w * scale;
	int height = h * scale;
	int xRatio = (int)((w << 16) / width) + 1;
	int yRatio = (int)((h << 16) / height) + 1;

	if ((flip & FLIP_X) == FLIP_X)
	{
		xPixelStart = width;
		xDirection = -1;
	}
	else
	{
		xPixelStart = 0;
		xDirection = 1;
	}

	if ((flip & FLIP_Y) == FLIP_Y)
	{
		yPixelStart = height;
		yDirection = -1;
	}
	else
	{
		yPixelStart = 0;
		yDirection = 1;
	}

	/* Pretty much running the nearest neighbor scaling on all blit pixels. Not really sure if this affects performance too much. */
	for (yFramebuffer = y, yPixel = yPixelStart; yFramebuffer < y + height; yFramebuffer++, yPixel += yDirection)
	{
		for (xFramebuffer = x, xPixel = xPixelStart; xFramebuffer < x + width; xFramebuffer++, xPixel += xDirection)
		{
			int xNearest = (xPixel * xRatio) >> 16;
			int yNearest = (yPixel * yRatio) >> 16;
			Color32 col = pixels[yNearest * pitch + xNearest];

			if (col != video->colorKey)
				milkPixelSet(video, xFramebuffer, yFramebuffer, col);
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
