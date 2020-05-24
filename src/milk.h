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

#define MILK_WINDOW_WIDTH MILK_FRAMEBUF_WIDTH * 3
#define MILK_WINDOW_HEIGHT MILK_FRAMEBUF_HEIGHT * 3

#define MILK_FRAMERATE (1000.0f / 50.0f)
#define MILK_FRAMEBUF_WIDTH 256
#define MILK_FRAMEBUF_HEIGHT 224

#define MILK_SPRSHEET_FILENAME "sprsheet.bmp"
#define MILK_FONT_FILENAME "font.bmp"
#define MILK_SPRSHEET_SQRSIZE 256
#define MILK_SPRSHEET_SPR_SQRSIZE 16
#define MILK_FONT_WIDTH 128
#define MILK_FONT_HEIGHT 48
#define MILK_CHAR_SQRSIZE 8

#define MILK_AUDIO_FREQUENCY 44100
#define MILK_AUDIO_CHANNELS 2 /* Stereo */
#define MILK_AUDIO_SAMPLES 4096
#define MILK_AUDIO_MAX_SOUNDS 25
#define MILK_AUDIO_QUEUE_MAX 16
#define MILK_AUDIO_MAX_VOLUME 128

#define MILK_MAX_LOGS 16
#define MILK_LOG_LENGTH 256

typedef struct
{
    void(*startTextInput)();
    void(*stopTextInput)();
    int(*readTextInput)(char *);
    int(*backspace)();
    int(*enter)();
    int(*escape)();
} System;

typedef enum
{
    INFO, WARN, ERROR
} LogType;

typedef struct
{
    size_t length;
    LogType type;
    char message[MILK_LOG_LENGTH];
} LogMessage;

typedef struct
{
    LogMessage messages[MILK_MAX_LOGS];
    int count;
} Logs;

typedef enum
{
    BTN_UP = 1 << 0,
    BTN_DOWN = 1 << 1,
    BTN_LEFT = 1 << 2,
    BTN_RIGHT = 1 << 3,
    BTN_A = 1 << 4,
    BTN_B = 1 << 5,
    BTN_X = 1 << 6,
    BTN_Y = 1 << 7
} ButtonState;

typedef struct
{
    ButtonState buttonState;
    ButtonState previousButtonState;
} Gamepad;

typedef struct
{
    uint32_t mouseX;
    uint32_t mouseY;
    uint8_t mouseDown;
    uint8_t mouseDownPrevious;
    Gamepad gamepad;
} Input;

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

typedef struct
{
    uint32_t length;
    uint8_t *buffer;
} SampleData;

typedef struct AudioQueueItem
{
    SampleData *sampleData;
    uint32_t remainingLength;
    uint8_t *position;
    uint8_t volume;
    uint8_t loop;
    uint8_t isFree;

    struct AudioQueueItem *next;
} AudioQueueItem;

typedef struct
{
    SampleData samples[MILK_AUDIO_MAX_SOUNDS];
    AudioQueueItem queueItems[MILK_AUDIO_QUEUE_MAX];
    AudioQueueItem *queue;
    uint8_t masterVolume;

    void(*lock)();
    void(*unlock)();
} Audio;

/*
 * Code is the client side script(s).
 * - state holds the current state of the program. In this case, it is a Lua state.
 */
typedef struct
{
	void *state;
} Code;

typedef struct
{
    System system;
    Logs logs;
	Input input;
    Video video;
    Audio audio;
	Code code;
} Milk;

Milk *milkInit();
void milkFree(Milk *milk);
void milkLog(Milk *milk, const char *message, LogType type);
void milkClearLogs(Milk *milk);
int milkHasError(Milk *milk);
void milkLoadSpritesheet(Video *video);
void milkLoadFont(Video *video);
int milkButton(Input *input, uint8_t button);
int milkButtonPressed(Input *input, uint8_t button);
void milkResetDrawState(Video *video);
void milkClipRect(Video *video, int x, int y, int w, int h);
void milkClear(Video *video, Color32 color);
void milkPixelSet(Video *video, int x, int y, Color32 color);
void milkRect(Video *video, int x, int y, int w, int h, Color32 color);
void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color);
void milkSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int flip);
void milkSpriteFont(Video *video, int x, int y, const char *str, float scale, Color32 color);
void milkSound(Audio *audio, int idx, uint8_t volume, uint8_t loop);
void milkVolume(Audio *audio, uint8_t volume);
void milkMixCallback(void *userdata, uint8_t *stream, int len);

#endif
