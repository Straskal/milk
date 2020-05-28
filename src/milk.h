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

#include <stdbool.h>
#include <stdint.h>

/*
 *******************************************************************************
 * Configuration
 *
 * Specs:
 * - 256x224 px resolution
 * - 256x256 px spritesheet, each sprite being 16x16 px (256 sprites total)
 * - 128x48  px bitmap font in ASCII order, starting from ASCII character 32 (space)
 * - 16 loaded sounds into memory
 * - 16 concurrent sounds, 1 of which can be looping.
 *******************************************************************************
 */

#define MILK_FRAMERATE              (1000.0f / 50.0f)
#define MILK_FRAMEBUF_WIDTH         256
#define MILK_FRAMEBUF_HEIGHT        224
#define MILK_WINDOW_WIDTH           MILK_FRAMEBUF_WIDTH * 3
#define MILK_WINDOW_HEIGHT          MILK_FRAMEBUF_HEIGHT * 3

#define MILK_SPRSHEET_FILENAME      "sprsheet.bmp"
#define MILK_FONT_FILENAME          "font.bmp"
#define MILK_SPRSHEET_SQRSIZE       256
#define MILK_SPRSHEET_SPR_SQRSIZE   16
#define MILK_FONT_WIDTH             128
#define MILK_FONT_HEIGHT            48
#define MILK_CHAR_SQRSIZE           8

#define MILK_AUDIO_FREQUENCY        44100
#define MILK_AUDIO_CHANNELS         2 /* Stereo */
#define MILK_AUDIO_SAMPLES          4096
#define MILK_AUDIO_MAX_SOUNDS       16
#define MILK_AUDIO_QUEUE_MAX        16
#define MILK_AUDIO_MAX_VOLUME       128

#define MILK_MAX_LOGS               16
#define MILK_LOG_MAX_LENGTH         512

/*
 *******************************************************************************
 * Logging
 *
 * As of right now, logging is pretty bare bones. Logs are only shown in the command line.
 *******************************************************************************
 */

typedef enum logType
{
    INFO, WARN, ERROR
} LogType;

typedef struct logMessage
{
    size_t  length;
    LogType type;
    char    text[MILK_LOG_MAX_LENGTH];
} LogMessage;

typedef struct logs
{
    LogMessage  messages[MILK_MAX_LOGS];
    int         count;
    int         errorCount;
} Logs;

/*
 *******************************************************************************
 * Input
 *
 * Milk supports a single gamepad with 9 buttons, each with a state of up and down.
 *******************************************************************************
 */

typedef enum buttonState
{
    BTN_START = 1 << 0,
    BTN_UP =    1 << 1,
    BTN_DOWN =  1 << 2,
    BTN_LEFT =  1 << 3,
    BTN_RIGHT = 1 << 4,
    BTN_A =     1 << 5,
    BTN_B =     1 << 6,
    BTN_X =     1 << 7,
    BTN_Y =     1 << 8,
} ButtonState;

typedef struct gamepad
{
    ButtonState buttonState;
    ButtonState previousButtonState;
} Gamepad;

typedef struct input
{
    Gamepad gamepad;
} Input;

/*
 *******************************************************************************
 * Video
 *
 * Internally, milk draws to it's own framebuffer, which is drawn to the screen at the end of every frame.
 * The framebuffer is just an array of pixels (32 bit color 0xAARRGGBB).
 *
 * The spritesheet and font are both loaded into memory during startup.
 * They are stored in fixed size arrays, so they do not need to be freed when milk shuts down.

 * Milk does not support transparency when drawing, but it does use a color key to consider as 'transparent', which defaults to black.
 * All drawing functions only operate within the bounds of the clipping rectangle, which is reset to the framebuffer size at the beginning of each frame.
 *******************************************************************************
 */

typedef uint32_t Color32;

typedef struct rect
{
    int top;
    int bottom;
    int left;
    int right;
} Rect;

typedef struct video
{
    Color32 framebuffer[MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT];
    Color32 spritesheet[MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SQRSIZE];
    Color32 font[MILK_FONT_WIDTH * MILK_FONT_HEIGHT];
    Color32 colorKey;
    Rect    clipRect;
} Video;

/*
 *******************************************************************************
 * Audio
 *
 * Sounds are dynamically allocated SampleData, and must be freed when they are no longer being used.
 *
 * Milk uses a queue like structure to store the currently playing sound instances.
 * The root queue item is a dynamically allocated dummy object. All other queue items act as a 'store', and are allocated on startup.
 * Many queue items can reference the same sample data, and sample data should be treated as readonly.
 *******************************************************************************
 */

typedef struct sampleData
{
    uint32_t length; /* Readonly */
    uint8_t *buffer; /* Readonly */
} SampleData;

typedef struct audioQueueItem
{
    SampleData *sampleData;
    uint32_t    remainingLength;
    uint8_t    *position;
    uint8_t     volume;
    bool        loop;
    bool        isFree;

    struct audioQueueItem *next;
} AudioQueueItem;

typedef struct audio
{
    SampleData      samples[MILK_AUDIO_MAX_SOUNDS];
    AudioQueueItem  queueItems[MILK_AUDIO_QUEUE_MAX];
    AudioQueueItem *queue;
    uint32_t        frequency;
    uint8_t         masterVolume;
    uint8_t         channels;

    void(*lock)();
    void(*unlock)();
} Audio;

/*
 *******************************************************************************
 * Code
 *
 * Milk's script state. It's pretty much completely abstracted away.
 *******************************************************************************
 */

typedef struct code
{
	void *state;
} Code;

/*
 *******************************************************************************
 * Milk
 *******************************************************************************
 */

typedef struct milk
{
    Logs    logs;
	Input   input;
    Video   video;
    Audio   audio;
	Code    code;
    bool    shouldQuit;
} Milk;

Milk *milkCreate();
void milkFree(Milk *milk);

void milkLog(Milk *milk, const char *text, LogType type);
void milkClearLogs(Milk *milk);

void milkLoadSpritesheet(Video *video);
void milkLoadFont(Video *video);

bool milkButton(Input *input, ButtonState button);
bool milkButtonPressed(Input *input, ButtonState button);

void milkResetDrawState(Video *video);
void milkClipRect(Video *video, int x, int y, int w, int h);
void milkClear(Video *video, Color32 color);
void milkPixelSet(Video *video, int x, int y, Color32 color);
void milkRect(Video *video, int x, int y, int w, int h, Color32 color);
void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color);
void milkSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int flip);
void milkSpriteFont(Video *video, int x, int y, const char *str, float scale, Color32 color);

void milkLoadSound(Audio *audio, int idx, const char *filename);
void milkSound(Audio *audio, int idx, uint8_t volume, uint8_t loop);
void milkVolume(Audio *audio, uint8_t volume);
void milkMixCallback(void *userdata, uint8_t *stream, int len);

#endif
