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
