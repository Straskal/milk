#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdint.h>

typedef struct
{
  uint32_t *pixels;
  int width;
  int height;
} Bitmap;

Bitmap *loadBitmap(const char *filePath);
void freeBitmap(Bitmap *bitmap);

#endif
