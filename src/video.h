/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdlib.h>

#include "common.h"

/*
 *******************************************************************************
 * Specification:
 * - [256x224] px resolution
 * - Fixed [50] fps
 * - [1] [256x256] px sprite sheet in memory at a time
 * - [1] [128x48] px bitmap font in memory at a time
 *
 * Notes:
 * Milk draws to an internal framebuffer in left->right, bottom->top order.
 * The framebuffer is just an array of pixels (32 bit color 0xAARRGGBB).
 * The sprite sheet and font are stored in fixed size, statically allocated arrays, so they do not need to be freed when milk shuts down. *
 * Milk does not support transparency when drawing, but it does use a color key to consider as 'transparent', which is defaulted to black.
 * All drawing functions only operate within the bounds of the clipping rectangle, which is reset to the framebuffer size at the beginning of each frame.
 * Platform code is responsible for reading milk's framebuffer and sending it on down to the graphics device, ya dig?
 *******************************************************************************
 */

#define FRAMERATE               (1000.0f / 50.0f)
#define FRAMEBUFFER_WIDTH       256
#define FRAMEBUFFER_HEIGHT      224
#define WINDOW_WIDTH            (FRAMEBUFFER_WIDTH * 3)
#define WINDOW_HEIGHT           (FRAMEBUFFER_HEIGHT * 3)
#define SPRITE_SHEET_SQRSIZE    256
#define SPRITE_SQRSIZE          16
#define FONT_WIDTH              128
#define FONT_HEIGHT             48
#define CHAR_WIDTH              8
#define CHAR_HEIGHT             8

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
    Color32 framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
    Color32 spriteSheet[SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE];
    Color32 font[FONT_WIDTH * FONT_HEIGHT];
    Color32 colorKey;
    Rect    clipRect;

    void(*loadBMP)(const char *, Color32 *, size_t);
} Video;

/**
 * Initialize the video submodule.
 * @param video
 */
void initVideo(Video *video);

/**
 * Load a sprite sheet into memory, replacing the current sprite sheet.
 * @param video
 * @param path
 */
void loadSpriteSheet(Video *video, const char *path);

/**
 * Load a font into memory, replace the current font.
 * @param video
 * @param path
 */
void loadFont(Video *video, const char *path);

/**
 * Reset video's draw state. This is to happen at the beginning of every new frame.
 * @param video
 */
void resetDrawState(Video *video);

/**
 * Set the current clipping rectangle. All drawing functions can only draw within its boundaries.
 * The clipping rectangle gets reset to the framebuffer size at the beginning of each frame.
 * @param video
 * @param x
 * @param y
 * @param w
 * @param h
 */
void setClippingRect(Video *video, int x, int y, int w, int h);

/**
 * Clear the framebuffer to the given color.
 * @param video
 * @param color
 */
void clearFramebuffer(Video *video, Color32 color);

/**
 * Blit a pixel to the framebuffer at the given coords.
 * @param video
 * @param x
 * @param y
 * @param color
 */
void blitPixel(Video *video, int x, int y, Color32 color);

/**
 * Blit a colored line to the framebuffer and the given coords.
 * @param video
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color);

/**
 * Blit a rectangle to the framebuffer at the given coords.
 * @param video
 * @param x
 * @param y
 * @param w
 * @param h
 * @param color
 */
void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color);

/**
 * Blit a solid rectangle to the framebuffer at the given coords.
 * @param video
 * @param x
 * @param y
 * @param w
 * @param h
 * @param color
 */
void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color);

/**
 * Blit a sprite at the given coordinates.
 * @param video
 * @param idx The index of the sprite on the sprite sheet.
 * @param x
 * @param y
 * @param w The number of sprites to span horizontally.
 * @param h The number of sprites to span vertically.
 * @param scale
 * @param flip 0 = no flip, 1 = flip horizontally, 2 = flip vertically, 3 = flip both
 */
void blitSprite(Video *video, int idx, int x, int y, int w, int h, int scale, u8 flip);

/**
 * Blit the given text in the current font.
 * @param video
 * @param pixels
 * @param x
 * @param y
 * @param str
 * @param scale
 * @param color
 */
void blitSpriteFont(Video *video, const Color32 *pixels, int x, int y, const char *str, int scale, Color32 color);

#endif
