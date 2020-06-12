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

#ifndef __MILK_H__
#define __MILK_H__

#include <stdbool.h>
#include <stdint.h>

/*
 *******************************************************************************
 * Current configuration:
 * - 256x224 px resolution.
 * - 256x256 px spritesheet, each sprite being 16x16 px (256 sprites total).
 * - 128x48  px bitmap font in ASCII order, starting from ASCII character 32 (space).
 * - Allows up to 16 sounds loaded into memory.
 * - Allows up to 16 concurrent sounds via sample slots. Index 0 loops.
 *******************************************************************************

 *******************************************************************************
 * Logging
 *
 * As of right now, logging is pretty bare bones. Logs are only shown in the command line.
 *******************************************************************************
 */

#define MAX_LOGS       16
#define MAX_LOG_LENGTH 512

#ifndef MILK_CMD
#define LOG_INFO(logs, text)    (void *)0;
#define LOG_WARN(logs, text)    (void *)0;
#define LOG_ERROR(logs, text)   (void *)0;
#else
#define LOG_INFO(milk, text)    logMessage(&milk->logs, text, INFO)
#define LOG_WARN(milk, text)    logMessage(&milk->logs, text, WARN)
#define LOG_ERROR(milk, text)   logMessage(&milk->logs, text, ERROR)
#endif

typedef enum logType
{
    INFO, WARN, ERROR
} LogType;

typedef struct logMessage
{
    LogType type;
    char    text[MAX_LOG_LENGTH];
} LogMessage;

typedef struct logs
{
    LogMessage  messages[MAX_LOGS];
    int         count;
    int         errorCount;
} Logs;

void logMessage(Logs *logs, const char *text, LogType type);
void clearLogs(Logs *logs);

/*
 *******************************************************************************
 * Input
 *
 * Milk supports a single gamepad with 9 buttons, each with a state of up and down.
 *******************************************************************************
 */

typedef enum buttonState
{
    BTN_START   = 1u << 0u,
    BTN_UP      = 1u << 1u,
    BTN_DOWN    = 1u << 2u,
    BTN_LEFT    = 1u << 3u,
    BTN_RIGHT   = 1u << 4u,
    BTN_A       = 1u << 5u,
    BTN_B       = 1u << 6u,
    BTN_X       = 1u << 7u,
    BTN_Y       = 1u << 8u,
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

bool isButtonDown(Input *input, ButtonState button);
bool isButtonPressed(Input *input, ButtonState button);

/*
 *******************************************************************************
 * Video
 *
 * Internally, milk draws to it's own framebuffer, which is drawn to the screen at the end of every frame.
 * The framebuffer is just an array of pixels (32 bit color 0xAARRGGBB).
 *
 * The spritesheet and font are stored in fixed size arrays, so they do not need to be freed when milk shuts down.
 *
 * Milk does not support transparency when drawing, but it does use a color key to consider as 'transparent', which defaults to black.
 * All drawing functions only operate within the bounds of the clipping rectangle, which is reset to the framebuffer size at the beginning of each frame.
 *******************************************************************************
 */

#define FRAMERATE               (1000.0f / 50.0f)
#define FRAMEBUFFER_HEIGHT      256
#define FRAMEBUFFER_WIDTH       224
#define WINDOW_WIDTH            (FRAMEBUFFER_HEIGHT * 3)
#define WINDOW_HEIGHT           (FRAMEBUFFER_WIDTH * 3)
#define SPRITE_SHEET_SQRSIZE    256
#define SPRITE_SQRSIZE          16
#define FONT_WIDTH              128
#define FONT_HEIGHT             48
#define CHAR_SQRSIZE            8

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
    Color32 framebuffer[FRAMEBUFFER_HEIGHT * FRAMEBUFFER_WIDTH];
    Color32 spriteSheet[SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE];
    Color32 font[FONT_WIDTH * FONT_HEIGHT];
    Color32 colorKey;
    Rect    clipRect;

    void(*loadBMP)(const char *, Color32 *, size_t);
} Video;

void loadSpriteSheet(Video *video, const char *path);
void loadFont(Video *video, const char *path);
void resetDrawState(Video *video);
void setClippingRect(Video *video, int x, int y, int w, int h);
void clearFramebuffer(Video *video, Color32 color);
void blitPixel(Video *video, int x, int y, Color32 color);
void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color);
void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color);
void blitSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int flip);
void blitSpriteFont(Video *video, const Color32 *pixels, int x, int y, const char *str, float scale, Color32 color);

/*
 *******************************************************************************
 * Audio
 *
 * Sounds are dynamically allocated SampleData, and must be freed when they are no longer being used.
 *
 * In order for a sound (sample data) to be played, it must be inserted into one of the sixteen sample slots.
 * Many sample slots can reference the same sample data.
 * Sample slot 0 loops.
 *******************************************************************************
 */

#define AUDIO_FREQUENCY     44100
#define AUDIO_CHANNELS      2 /* Stereo */
#define AUDIO_SAMPLES       4096
#define MAX_LOADED_SAMPLES  16
#define MAX_SAMPLE_SLOTS    16
#define MAX_VOLUME          128

typedef struct sampleData
{
    uint32_t length;
    uint8_t *buffer;
} SampleData;

typedef enum sampleSlotState
{
    STOPPED,
    PLAYING,
    PAUSED
} SampleSlotState;

typedef struct sampleSlot
{
    SampleData     *sampleData;
    SampleSlotState state;
    uint32_t        remainingLength;
    uint8_t        *position;
    uint8_t         volume;
} SampleSlot;

typedef struct audio
{
    SampleData      samples[MAX_LOADED_SAMPLES];
    SampleSlot      slots[MAX_SAMPLE_SLOTS];
    uint32_t        frequency;
    uint8_t         masterVolume;
    uint8_t         channels;

    void(*loadWAV)(struct audio *, const char *, int);
    void(*lock)();
    void(*unlock)();
} Audio;

void loadSound(Audio *audio, int idx, const char *filename);
void unloadSound(Audio *audio, int idx);
void playSound(Audio *audio, int sampleIdx, int slotIdx, int volume);
void stopSound(Audio *audio, int slotIdx);
void pauseSound(Audio *audio, int slotIdx);
void resumeSound(Audio *audio, int slotIdx);
SampleSlotState getSampleState(Audio *audio, int slotIdx);
void setMasterVolume(Audio *audio, int volume);
void mixSamplesIntoStream(Audio *audio, uint8_t *stream, int len);

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

Milk *createMilk();
void freeMilk(Milk *milk);

#endif
