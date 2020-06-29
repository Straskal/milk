#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdlib.h>

#include "common.h"

// Run at a fixed 50 frames per second. 50 fps just sounds cool, doesn't it? It's not 60 fps, but it's still good.
#define FRAMERATE (1000.0f / 50.0f)

// Fixed 256x224 pixel resolution. Same as the SNES, and the SNES rocks.
#define FRAMEBUFFER_WIDTH   256
#define FRAMEBUFFER_HEIGHT  224

// We want our window size to the a bit larger than our fixed resolution.
#define WINDOW_WIDTH  (FRAMEBUFFER_WIDTH * 3)
#define WINDOW_HEIGHT (FRAMEBUFFER_HEIGHT * 3)

// Sprite sheets must be 256x256 px images.
// Each individual sprite is considered to be 16x16 px.
// This gives us 256 sprites per sprite sheet.
#define SPRITE_SHEET_SQRSIZE  256
#define SPRITE_SQRSIZE        16

// Bitmap fonts must be 128x48 px images.
// Characters must be in ASCII order from left->right.
// Each character is considered to be 8x8 px.
// We don't use fixed, squared values because fonts need some leeway.
#define FONT_WIDTH  128
#define FONT_HEIGHT 48
#define CHAR_WIDTH  8
#define CHAR_HEIGHT 8

// A color/pixel is represented by a packed 32 bit integer: 0xAARRGGBB
// The alpha value is ignored.
typedef u32 Color32;

typedef struct rect
{
	int top;
	int bottom;
	int left;
	int right;
} Rect;

typedef struct video
{
  // Milk's framebuffer is just an array of 32 bit colors.
  // All drawing operations to the framebuffer are in [left->right : top->bottom] order.
  // Platform code is resposible getting milk's framebuffer onto the screen.
	Color32 framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

  // A single sprite sheet is allowed in memory at a time, but can be swapped out for another.
	Color32 spriteSheet[SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE];

  // A single bitmap font is allowed in memory at a time, but can be swapped out for another.
	Color32 font[FONT_WIDTH * FONT_HEIGHT];

  // The color key is what milk's drawing operations consider to be 'transparent'.
  // Pixels that match the color of the color key are not drawn.
	Color32 colorKey;

  // All drawing operations work within the bounds of the clipping rectangle.
	Rect clipRect;

	void(*loadBMP)(const char *, Color32 *, size_t);
} Video;

// Initialize the video submodule.
void initVideo(Video *video);

// Load a sprite sheet into memory, replacing the current sprite sheet.
void loadSpriteSheet(Video *video, const char *path);

// Load a font into memory, replace the current font.
void loadFont(Video *video, const char *path);

// Reset video's draw state. This is to happen at the beginning of every new frame.
void resetDrawState(Video *video);

// Set the current clipping rectangle.
// The clipping rectangle gets reset to the framebuffer size at the beginning of each frame.
void setClippingRect(Video *video, int x, int y, int w, int h);

// Clear the framebuffer to the given color.
void clearFramebuffer(Video *video, Color32 color);

// Blit a pixel to the framebuffer at the given coords.
void blitPixel(Video *video, int x, int y, Color32 color);

// Blit a colored line to the framebuffer and the given coords.
void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color);

// Blit a colored rectangle to the framebuffer at the given coords.
void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color);

// Blit a solid colored rectangle to the framebuffer at the given coords.
void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color);

// Blit a sprite to the given coordinates.
// Specify which sprite to be drawin via the idx param.
// Specify the width and height (in sprites) to be draw. This is useful for drawing sprites that are larger than 16x16px.
// Specify the scale at which the sprite should be drawn.
// Specify whether or not the sprite should be flipped. 0 = no flip, 1 = flip horizontally, 2 = flip vertically, 3 = flip both
void blitSprite(Video *video, int idx, int x, int y, int w, int h, int scale, u8 flip);

// Blit the given text to the screen.
void blitSpriteFont(Video *video, const Color32 *pixels, int x, int y, const char *str, int scale, Color32 color);

#endif
