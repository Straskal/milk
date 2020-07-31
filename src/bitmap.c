#include <SDL.h>

#include "bitmap.h"

Bitmap *loadBitmap(const char *filePath) {
  SDL_Surface *surface = SDL_LoadBMP(filePath);
  if (!surface)
    return NULL;
  if (surface->format->format != SDL_PIXELFORMAT_BGR24) {
    SDL_FreeSurface(surface);
    return NULL;
  }
  int length = surface->w * surface->h;
  Bitmap *bitmap = malloc(sizeof(Bitmap));
  bitmap->pixels = malloc(length * sizeof(uint32_t));
  bitmap->width = surface->w;
  bitmap->height = surface->h;
  uint8_t *surfacePixels = surface->pixels;
  for (int i = 0; i < length; i++) {
    uint32_t b = *surfacePixels++;
    uint32_t g = *surfacePixels++;
    uint32_t r = *surfacePixels++;
    bitmap->pixels[i] = (r << 16) | (g << 8) | (b);
  }
  SDL_FreeSurface(surface);
  return bitmap;
}

void freeBitmap(Bitmap *bitmap) {
  free(bitmap->pixels);
  free(bitmap);
}
