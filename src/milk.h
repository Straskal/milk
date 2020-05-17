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

#ifndef __MILK_H__
#define __MILK_H__

#include <stdint.h>

/* Frame rate */
#define MILK_FRAMERATE (1000.0f / 50.0f)

/* Audio */
#define MILK_AUDIO_FREQUENCY 44100
#define MILK_AUDIO_CHANNELS 2 /* Stereo */
#define MILK_AUDIO_SAMPLES 4096
#define MILK_AUDIO_MAX 25
#define MILK_AUDIO_QUEUE_MAX 16

/* Frame buffer */
#define MILK_FRAMEBUF_WIDTH 256
#define MILK_FRAMEBUF_HEIGHT 224

/* Window */
#define MILK_WINDOW_WIDTH MILK_FRAMEBUF_WIDTH * 3
#define MILK_WINDOW_HEIGHT MILK_FRAMEBUF_HEIGHT * 3

/* Sprite sheet */
#define MILK_SPRSHEET_SQRSIZE 256
#define MILK_SPRSHEET_SPR_SQRSIZE 16
#define MILK_SPRSHEET_ROWS (MILK_SPRSHEET_SQRSIZE / MILK_SPRSHEET_SPR_SQRSIZE)
#define MILK_SPRSHEET_FILENAME "sprsheet.bmp"

/* Font */
#define MILK_FONT_FILENAME "font.bmp"
#define MILK_FONT_WIDTH 128
#define MILK_FONT_HEIGHT 48
#define MILK_CHAR_SQRSIZE 8

#define MILK_BOOL int
#define MILK_TRUE 1
#define MILK_FALSE 0

/* Sample data for sounds. */
typedef struct SampleData
{
    uint32_t length;
    uint8_t *buffer;
} SampleData;

/* A queue of active samples. */
typedef struct AudioQueueItem
{
    SampleData *sampleData;
    uint32_t remainingLength;
    uint8_t *position;
    uint8_t volume;
    uint8_t isMusic;
    uint8_t isFading;
    uint8_t isFree;

    struct AudioQueueItem *next;
} AudioQueueItem;

/* Since we're using an async callback system, we must lock and unlock the audio device when manipulating the audio queue. */
typedef struct Audio
{
    SampleData samples[MILK_AUDIO_MAX];
    AudioQueueItem queueItems[MILK_AUDIO_QUEUE_MAX];
    AudioQueueItem *queue;
    uint8_t masterVolume;
    uint8_t musicVolume;
    uint8_t soundVolume;
    void(*lock)();
    void(*unlock)();
} Audio;

/* 24 bit color is packed into 32 bits: 0x00RRGGBB */
typedef uint32_t Color32;

#define COLOR_R(c) (c >> 24)
#define COLOR_G(c) (c >> 16)
#define COLOR_B(c) (c >> 8)

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

enum
{
	BTN_UP      = 1 << 0,
	BTN_DOWN    = 1 << 1,
	BTN_LEFT    = 1 << 2,
	BTN_RIGHT   = 1 << 3,
	BTN_A       = 1 << 4,
	BTN_B       = 1 << 5,
	BTN_X       = 1 << 6,
	BTN_Y       = 1 << 7
};

typedef struct Gamepad
{
	uint8_t buttonState;
    uint8_t previousButtonState;
} Gamepad;

typedef struct Input
{
	uint32_t mouseX;
	uint32_t mouseY;
	MILK_BOOL mouseDown;
	MILK_BOOL mouseDownPrevious;
	Gamepad gamepad;
} Input;

/*
 * Code is the client side script(s).
 * - state holds the current state of the program. In this case, it is a Lua state.
 */
typedef struct Code
{
	void *state;
} Code;

typedef struct Milk
{
	Input input;
    Audio audio;
	Video video;
	Code code;
} Milk;

Milk *milkInit();
void milkFree(Milk *milk);
void milkUpdate(Milk *milk);
void milkDraw(Milk *milk);
int milkButton(Input *input, uint8_t button);
int milkButtonPressed(Input *input, uint8_t button);
void milkPlayMusic(Audio *audio, int idx);
void milkSound(Audio *audio, int idx, uint8_t volume);
void milkClipRect(Video *video, int x, int y, int w, int h);
void milkClear(Video *video, Color32 idx);
void milkPixelSet(Video *video, int x, int y, Color32 color);
void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color);
void milkRect(Video *video, int x, int y, int w, int h, Color32 color);
void milkSprite(Video *video, int idx, int x, int y, int w, int h, float scale);
void milkSpriteFont(Video *video, int x, int y, const char *str, float scale);

#endif
