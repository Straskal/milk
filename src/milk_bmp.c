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

#include "milk_bmp.h"
#include "SDL.h"

static void _loadBmp(Color32 *dest, char *filename)
{
	SDL_Surface *bmp = SDL_LoadBMP(filename);
	int bpp = bmp->format->BytesPerPixel;
	int pitch = bmp->pitch;
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

void milkLoadSpritesheet(Color32 *spritesheet, char *filename)
{
	_loadBmp(spritesheet, filename);
}

void milkLoadFont(Color32 *font, char *filename)
{
	_loadBmp(font, filename);
}
