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

#define FRAMEBUFFER_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x) /* xy coords to framebuffer pixel index. */
#define WITHIN_CLIP_RECT(clip, x, y) (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)
#define FRAMEBUFFER_MIN(x) (x < 0 ? 0 : x)
#define FRAMEBUFFER_MAXX(x) (x > MILK_FRAMEBUF_WIDTH ? MILK_FRAMEBUF_WIDTH : x)
#define FRAMEBUFFER_MAXY(y) (y > MILK_FRAMEBUF_HEIGHT ? MILK_FRAMEBUF_HEIGHT : y)

#define FLIPX 1
#define FLIPY 2

#define IS_ASCII(c) ((c & 0xff80) == 0)
#define IS_NEWLINE(c) (c == '\n')

/* Load bitmap and copy it's pixel data into a fixed size buffer. */
/* Would also love to rid of this SDL dependency. Could have custom bmp loading here. */
static void _loadBitmap(char *filename, Color32 *dest, size_t len)
{
	SDL_Surface *bmp = SDL_LoadBMP(filename);
	uint8_t *bmpPixels = (Uint8 *)bmp->pixels;

	for (int i = 0; i < len; i++)
	{
		int b = *(bmpPixels++);
		int g = *(bmpPixels++);
		int r = *(bmpPixels++);

		dest[i] = (r << 16) | (g << 8) | (b);
	}

	SDL_FreeSurface(bmp);
}

void milkOpenVideo(Video *video)
{
	_loadBitmap(MILK_SPRSHEET_FILENAME, &video->spritesheet, MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SQRSIZE);
	_loadBitmap(MILK_FONT_FILENAME, &video->font, MILK_FONT_WIDTH * MILK_FONT_HEIGHT);
}

void milkClipRect(Video *video, int x, int y, int w, int h)
{
	int right = x + w;
	int bottom = y + h;

	x = FRAMEBUFFER_MIN(x);
	x = FRAMEBUFFER_MAXX(x);
	right = FRAMEBUFFER_MIN(right);
	right = FRAMEBUFFER_MAXX(right);
	y = FRAMEBUFFER_MIN(y);
	y = FRAMEBUFFER_MAXY(y);
	bottom = FRAMEBUFFER_MIN(bottom);
	bottom = FRAMEBUFFER_MAXY(bottom);
	video->clipRect.left = x;
	video->clipRect.right = right;
	video->clipRect.top = y;
	video->clipRect.bottom = bottom;
}

void milkClear(Video *video, Color32 color)
{
	Rect clip = video->clipRect;

	for (int i = clip.top; i < clip.bottom; i++)
	{
		for (int j = clip.left; j < clip.right; j++)
			video->framebuffer[FRAMEBUFFER_POS(j, i)] = color;
	}
}

void milkPixelSet(Video *video, int x, int y, Color32 color)
{
	if (WITHIN_CLIP_RECT(video->clipRect, x, y))
		video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

static void _horizontalLine(Video *video, int x, int y, int w, Color32 color)
{
	for (int i = x; i <= x + w; i++)
		milkPixelSet(video, i, y, color);
}

static void _verticalLine(Video *video, int x, int y, int h, Color32 color)
{
	for (int i = y; i <= y + h; i++)
		milkPixelSet(video, x, i, color);
}

void milkRect(Video *video, int x, int y, int w, int h, Color32 color)
{
	_horizontalLine(video, x, y, w, color);
	_horizontalLine(video, x, y + h, w, color);
	_verticalLine(video, x, y, h, color);
	_verticalLine(video, x + w, y, h, color);
}

void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color)
{
	for (int j = y; j < y + h; j++)
	{
		for (int i = x; i < x + w; i++)
			milkPixelSet(video, i, j, color);
	}
}

/*
 * Blit a rectangular pixel buffer onto the video's frame buffer.
 * Allows for scaling and flipping.
 */
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

	if ((flip & FLIPX) == FLIPX)
	{
		xPixelStart = width - 1;
		xDirection = -1;
	}
	else
	{
		xPixelStart = 0;
		xDirection = 1;
	}

	if ((flip & FLIPY) == FLIPY)
	{
		yPixelStart = height - 1;
		yDirection = -1;
	}
	else
	{
		yPixelStart = 0;
		yDirection = 1;
	}

	/* Pretty much running the nearest neighbor scaling on all blit pixels. This doesn't seem to affect performance. */
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

void milkSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int flip)
{
	static int numColumns = MILK_SPRSHEET_SQRSIZE / MILK_SPRSHEET_SPR_SQRSIZE;
	static int rowSize = MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SPR_SQRSIZE;
	static int colSize = MILK_SPRSHEET_SPR_SQRSIZE;

	if (idx < 0 || MILK_SPRSHEET_SQRSIZE < idx)
		return;

	int row = (int)floor(idx / numColumns);
	int col = (int)floor(idx % numColumns);
	Color32 *pixels = &video->spritesheet[row * rowSize + col * colSize];

	_blitRect(video, pixels, x, y, w * MILK_SPRSHEET_SPR_SQRSIZE, h * MILK_SPRSHEET_SPR_SQRSIZE, MILK_SPRSHEET_SQRSIZE, scale, flip);
}

static void _drawCharacter(Video *video, int x, int y, char character, float scale)
{
	static int numColumns = MILK_FONT_WIDTH / MILK_CHAR_SQRSIZE;
	static int rowSize = MILK_FONT_WIDTH * MILK_CHAR_SQRSIZE;
	static int colSize = MILK_CHAR_SQRSIZE;

	if (!IS_ASCII(character))
		character = '?';

	/* bitmap font starts at ASCII character 32 (SPACE) */
	int row = (int)floor((character - 32) / numColumns);
	int col = (int)floor((character - 32) % numColumns);
	Color32 *pixels = &video->font[(row * rowSize + col * colSize)];

	_blitRect(video, pixels, x, y, MILK_CHAR_SQRSIZE, MILK_CHAR_SQRSIZE, MILK_FONT_WIDTH, scale, 0);
}

void milkSpriteFont(Video *video, int x, int y, const char *str, float scale)
{
	int size = MILK_CHAR_SQRSIZE * scale;
	int xCurrent = x;
	int yCurrent = y;

	while (*str)
	{
		while (IS_NEWLINE(*str))
		{
			xCurrent = x;
			yCurrent += size;
			str++;
		}

		_drawCharacter(video, xCurrent, yCurrent, *(str++), scale);
		xCurrent += size;
	}
}
