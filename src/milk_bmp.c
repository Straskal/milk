#include "milk_bmp.h"
#include "SDL.h"

static void _loadBmp(ColorRGB *pixels, char *filename)
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
	ColorRGB *pixelitr = pixels;
	while (itr != end)
	{
		pixelitr->b = *(itr++);
		pixelitr->g = *(itr++);
		pixelitr->r = *(itr++);
		pixelitr++;
	}
	SDL_FreeSurface(bmp);
}

void milkLoadSpritesheet(Milk *milk, char *filename)
{
	_loadBmp(milk->video.spritesheet, filename);
}

void milkLoadFont(Milk *milk, char *filename)
{
	_loadBmp(milk->video.font, filename);
}
