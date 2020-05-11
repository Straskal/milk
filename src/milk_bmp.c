#include "milk_bmp.h"
#include "SDL.h"

void milkLoadSpritesheet(Milk *milk, char *filename)
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
	ColorRGB *pixelitr = &milk->video.spritesheet;
	while (itr != end)
	{
		pixelitr->b = *(itr++);
		pixelitr->g = *(itr++);
		pixelitr->r = *(itr++);
		pixelitr++;
	}
	SDL_FreeSurface(bmp);
}
