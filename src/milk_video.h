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

#ifndef __MILK_VIDEO_H__
#define __MILK_VIDEO_H__

#include <stdint.h>

#define MILK_FRAMERATE (1000.0f / 50.0f)

#define MILK_FRAMEBUF_WIDTH 256
#define MILK_FRAMEBUF_HEIGHT 224

#define MILK_SPRSHEET_SQRSIZE 256
#define MILK_SPRSHEET_SPR_SQRSIZE 16
#define MILK_SPRSHEET_FILENAME "sprsheet.bmp"

#define MILK_FONT_FILENAME "font.bmp"
#define MILK_FONT_WIDTH 128
#define MILK_FONT_HEIGHT 48
#define MILK_CHAR_SQRSIZE 8

/* Packed 32 bit color: 0xAARRGGBB */
typedef uint32_t Color32;

typedef struct Rect
{
    int top;
    int bottom;
    int left;
    int right;
} Rect;

/*
 * - The framebuffer is what we actively draw to, and display at the end of each frame.
 * - The sprite sheet is a single image px buffer.
 * - The font is a single image px buffer.
 * - When blitting sprites or fonts, any pixels matching the color key will not be drawn.
 */
typedef struct Video
{
    Color32 framebuffer[MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT];
    Color32 spritesheet[MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SQRSIZE];
    Color32 font[MILK_FONT_WIDTH * MILK_FONT_HEIGHT];
    Color32 colorKey;
    Rect clipRect;
} Video;

void milkOpenVideo(Video *video);
void milkClipRect(Video *video, int x, int y, int w, int h);
void milkClear(Video *video, Color32 color);
void milkPixelSet(Video *video, int x, int y, Color32 color);
void milkRect(Video *video, int x, int y, int w, int h, Color32 color);
void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color);
void milkSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int flip);
void milkSpriteFont(Video *video, int x, int y, const char *str, float scale);

#endif
