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
 * Specifications:
 * - 256x224px resolution
 * - Fixed 50fps
 * - 1 256x256px sprite sheet in memory at a time
 * - 1 128x48px bitmap font in memory at a time
 *
 * Milk draws to an internal framebuffer.
 * The framebuffer is just an array of pixels (32 bit color 0xAARRGGBB). *
 * The sprite sheet and font are stored in fixed size arrays, so they do not need to be freed when milk shuts down. *
 * Milk does not support transparency when drawing, but it does use a color key to consider as 'transparent', which is defaulted to black.
 * All drawing functions only operate within the bounds of the clipping rectangle, which is reset to the framebuffer size at the beginning of each frame.
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

void initVideo(Video *video);

void loadSpriteSheet(Video *video, const char *path);

void loadFont(Video *video, const char *path);

void resetDrawState(Video *video);

void setClippingRect(Video *video, int x, int y, int w, int h);

void clearFramebuffer(Video *video, Color32 color);

void blitPixel(Video *video, int x, int y, Color32 color);

void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color);

void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color);

void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color);

void blitSprite(Video *video, int idx, int x, int y, int w, int h, int scale, u8 flip);

void blitSpriteFont(Video *video, const Color32 *pixels, int x, int y, const char *str, int scale, Color32 color);

#endif
