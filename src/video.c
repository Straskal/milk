#include <math.h>
#include <string.h>

#include "embed/font.h"
#include "video.h"

void initVideo(Video *video)
{
	memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
	memset(&video->spriteSheet, 0x00, sizeof(video->spriteSheet));
	memcpy(&video->font, DEFAULT_FONT_DATA, sizeof(video->font));
	resetDrawState(video);
}

void loadSpriteSheet(Video *video, const char *path)
{
	video->loadBMP(path, video->spriteSheet, sizeof(video->spriteSheet) / sizeof(Color32));
}

void loadFont(Video *video, const char *path)
{
	video->loadBMP(path, video->font, sizeof(video->font) / sizeof(Color32));
}

void resetDrawState(Video *video)
{
	video->colorKey = 0x00;
	video->clipRect.top = 0;
	video->clipRect.left = 0;
	video->clipRect.bottom = FRAMEBUFFER_HEIGHT;
	video->clipRect.right = FRAMEBUFFER_WIDTH;
}

void setClippingRect(Video *video, int x, int y, int w, int h)
{
	video->clipRect.left = CLAMP(x, 0, FRAMEBUFFER_WIDTH);
	video->clipRect.right = CLAMP(x + w, 0, FRAMEBUFFER_WIDTH);
	video->clipRect.top = CLAMP(y, 0, FRAMEBUFFER_HEIGHT);
	video->clipRect.bottom = CLAMP(y + h, 0, FRAMEBUFFER_HEIGHT);
}

#define FRAMEBUFFER_POS(x, y)           ((FRAMEBUFFER_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y)    (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

void clearFramebuffer(Video *video, Color32 color)
{
	Rect clip = video->clipRect;

	for (int i = clip.top; i < clip.bottom; i++)
	{
		for (int j = clip.left; j < clip.right; j++)
			video->framebuffer[FRAMEBUFFER_POS(j, i)] = color;
	}
}

void blitPixel(Video *video, int x, int y, Color32 color)
{
	if (WITHIN_CLIP_RECT(video->clipRect, x, y))
		video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

static void bresenhamLine(Video *video, int x0, int y0, int x1, int y1, Color32 color)
{
	int xDistance = x1 - x0;
	int yDistance = y1 - y0;
	int xStep = SIGN(xDistance);
	int yStep = SIGN(yDistance);
	xDistance = abs(xDistance) << 1;
	yDistance = abs(xDistance) << 1;
	blitPixel(video, x0, y0, color);

	if (xDistance > yDistance)
	{
		int fraction = yDistance - (xDistance >> 1);
		while (x0 != x1)
		{
			x0 += xStep;
			if (fraction >= 0)
			{
				y0 += yStep;
				fraction -= xDistance;
			}
			fraction += yDistance;
			blitPixel(video, x0, y0, color);
		}
	}
	else
	{
		int fraction = xDistance - (yDistance >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += xStep;
				fraction -= yDistance;
			}
			y0 += yStep;
			fraction += xDistance;
			blitPixel(video, x0, y0, color);
		}
	}
}

void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color)
{
	bresenhamLine(video, x0, y0, x1, y1, color);
}

static void horizontalLine(Video *video, int x, int y, int w, Color32 color)
{
	for (int i = x; i <= x + w; i++)
		blitPixel(video, i, y, color);
}

static void verticalLine(Video *video, int x, int y, int h, Color32 color)
{
	for (int i = y; i <= y + h; i++)
		blitPixel(video, x, i, color);
}

void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
	horizontalLine(video, x, y, w, color); /* Top edge */
	horizontalLine(video, x, y + h, w, color); /* Bottom edge */
	verticalLine(video, x, y, h, color); /* Left edge */
	verticalLine(video, x + w, y, h, color); /* Right edge */
}

void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
	for (int i = y; i < y + h; i++)
	{
		for (int j = x; j < x + w; j++)
			blitPixel(video, j, i, color);
	}
}

#define MIN_SCALE						1
#define MAX_SCALE						5
#define IS_FLIPPED_X(flip)	(flip & 1)
#define IS_FLIPPED_Y(flip)	(flip & 2)

static void nearestNeighbor(Video *video, const Color32 *pixels, int x, int y, int w, int h, int pitch, int scale, u8 flip, const Color32 *color)
{
	scale = CLAMP(scale, MIN_SCALE, MAX_SCALE);

	int width = w * scale;
	int height = h * scale;
	int xRatio = (int)floor((double)(w << 16) / width + 0.5);
	int yRatio = (int)floor((double)(h << 16) / height + 0.5);
	int xPixelStart = IS_FLIPPED_X(flip) ? width - 1 : 0;
	int yPixelStart = IS_FLIPPED_Y(flip) ? height - 1 : 0;
	int xStep = IS_FLIPPED_X(flip) ? -1 : 1;
	int yStep = IS_FLIPPED_Y(flip) ? -1 : 1;

	int xPixel, yPixel, xFramebuffer, yFramebuffer;
	for (yFramebuffer = y, yPixel = yPixelStart; yFramebuffer < y + height; yFramebuffer++, yPixel += yStep)
	{
		for (xFramebuffer = x, xPixel = xPixelStart; xFramebuffer < x + width; xFramebuffer++, xPixel += xStep)
		{
			int xNearest = (xPixel * xRatio) >> 16;
			int yNearest = (yPixel * yRatio) >> 16;
			Color32 col = pixels[yNearest * pitch + xNearest];

			if (col != video->colorKey)
				blitPixel(video, xFramebuffer, yFramebuffer, color != NULL ? *color : col);
		}
	}
}

static void blitRect(Video *video, const Color32 *pixels, int x, int y, int w, int h, int pitch, int scale, u8 flip, const Color32 *color)
{
	nearestNeighbor(video, pixels, x, y, w, h, pitch, scale, flip, color);
}

#define SPRSHEET_IDX_OO_BOUNDS(idx)	(idx < 0 || SPRITE_SHEET_SQRSIZE < idx)
#define SPRSHEET_COLUMNS						((int)(SPRITE_SHEET_SQRSIZE / SPRITE_SQRSIZE))
#define SPRSHEET_ROW_SIZE						((int)(SPRITE_SHEET_SQRSIZE * SPRITE_SQRSIZE))
#define SPRSHEET_COL_SIZE						SPRITE_SQRSIZE
#define SPRSHEET_POS(x, y)					(y * SPRSHEET_ROW_SIZE + x * SPRSHEET_COL_SIZE)

void blitSprite(Video *video, int idx, int x, int y, int w, int h, int scale, u8 flip)
{
	if (SPRSHEET_IDX_OO_BOUNDS(idx))
		return;

	int width = w * SPRITE_SQRSIZE;
	int height = h * SPRITE_SQRSIZE;
	int row = (int)floor((double)idx / SPRSHEET_COLUMNS);
	int col = (int)floor((double)(idx % SPRSHEET_COLUMNS));
	Color32 *pixels = &video->spriteSheet[SPRSHEET_POS(col, row)];
	blitRect(video, pixels, x, y, width, height, SPRITE_SHEET_SQRSIZE, scale, flip, NULL);
}

#define FONT_COLUMNS		((int)(FONT_WIDTH / CHAR_WIDTH))
#define FONT_ROW_SIZE		((int)(FONT_WIDTH * CHAR_HEIGHT))
#define FONT_POS(x, y)	(y * FONT_ROW_SIZE + x * CHAR_WIDTH)
#define IS_ASCII(c)			(0 < c)
#define IS_NEWLINE(c)		(c == '\n')

void blitSpriteFont(Video *video, const Color32 *pixels, int x, int y, const char *str, int scale, Color32 color)
{
	if (str == NULL)
		return;

	int charWidth = (int)floor((double)CHAR_WIDTH * scale);
	int charHeight = (int)floor((double)CHAR_HEIGHT * scale);
	int xCurrent = x;
	int yCurrent = y;
	char curr;

	while ((curr = *str++) != '\0')
	{
		if (!IS_NEWLINE(curr))
		{
			if (!IS_ASCII(curr)) curr = '?';
			int row = (int)floor((double)(curr - 32) / FONT_COLUMNS); /* bitmap font starts at ASCII character 32 (SPACE) */
			int col = (int)floor((double)((curr - 32) % FONT_COLUMNS));
			const Color32 *pixelStart = &pixels[FONT_POS(col, row)];
			blitRect(video, pixelStart, xCurrent, yCurrent, CHAR_WIDTH, CHAR_HEIGHT, FONT_WIDTH, scale, 0, &color);
			xCurrent += charWidth;
		}
		else
		{
			xCurrent = x;
			yCurrent += charHeight;
		}
	}
}
