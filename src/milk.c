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

#include "milk.h"
#include "embed/font.h"

#include <math.h>
#include <string.h>

/*
 *******************************************************************************
 * Helpers
 *******************************************************************************
 */


#define MIN(x, y)           ((x) > (y) ? (y) : (x))
#define MAX(x, y)           ((x) > (y) ? (x) : (y))
#define CLAMP(v, low, up)   (MAX(low, MIN(v, up)))
#define SIGN(x)             ((x > 0) - (x < 0))


/*
 *******************************************************************************
 * Initialization and shutdown
 *******************************************************************************
 */


static void initLogs(Logs *logs)
{
    for (int i = 0; i < MAX_LOGS; i++)
    {
        logs->messages[i].type = INFO;
        memset(logs->messages[i].text, 0, MAX_LOG_LENGTH);
    }

    logs->count = 0;
    logs->errorCount = 0;
}


static void initInput(Input *input)
{
    input->gamepad.buttonState = BTN_NONE;
    input->gamepad.previousButtonState = BTN_NONE;
}


static void initVideo(Video *video)
{
    memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
    memset(&video->spriteSheet, 0x00, sizeof(video->spriteSheet));
    memcpy(&video->font, DEFAULT_FONT_DATA, sizeof(video->font));
    resetDrawState(video);
}


static void initAudio(Audio *audio)
{
    for (int i = 0; i < MAX_LOADED_SAMPLES; i++)
    {
        audio->samples[i].buffer = NULL;
        audio->samples[i].length = 0;
    }

    for (int i = 0; i < MAX_SAMPLE_SLOTS; i++)
    {
        audio->slots[i].sampleData = NULL;
        audio->slots[i].state = STOPPED;
        audio->slots[i].remainingLength = 0;
        audio->slots[i].position = NULL;
        audio->slots[i].volume = 0;
    }

    audio->masterVolume = MAX_VOLUME;
    audio->frequency = 0;
    audio->channels = 0;
}


static void initCode(Code *code)
{
    code->state = NULL;
}


Milk *createMilk()
{
    Milk *milk = (Milk *) malloc(sizeof(Milk));
    milk->shouldQuit = false;
    initLogs(&milk->logs);
    initInput(&milk->input);
    initVideo(&milk->video);
    initAudio(&milk->audio);
    initCode(&milk->code);
    return milk;
}


void freeMilk(Milk *milk)
{
    for (int i = 0; i < MAX_LOADED_SAMPLES; i++)
        free(milk->audio.samples[i].buffer);

    free(milk);
}


/*
 *******************************************************************************
 * Logging
 *******************************************************************************
 */


/* When milk's log array is full, we shift down all of the logs before inserting the next one.*/
static LogMessage *getNextFreeLogMessage(Logs *logs)
{
    if (logs->count == MAX_LOGS)
    {
        for (int i = 0; i < MAX_LOGS - 1; i++)
            logs->messages[i] = logs->messages[i + 1];

        return &logs->messages[MAX_LOGS - 1];
    }
    else
        return &logs->messages[logs->count++];
}


void logMessage(Logs *logs, const char *text, LogType type)
{
    size_t len = strlen(text);

    if (len > MAX_LOG_LENGTH)
        len = MAX_LOG_LENGTH;

    if (type == ERROR)
        logs->errorCount++;

    LogMessage *newLogMessage = getNextFreeLogMessage(logs);
    memset(newLogMessage->text, 0, MAX_LOG_LENGTH);
    strncpy(newLogMessage->text, text, len);
    newLogMessage->type = type;
}


void clearLogs(Logs *logs)
{
    logs->count = 0;
    logs->errorCount = 0;
}


/*
 *******************************************************************************
 * Input
 *******************************************************************************
 */


#define IS_BIT_SET(val, bit) ((val & bit))


bool isButtonDown(Input *input, ButtonState button)
{
    return IS_BIT_SET(input->gamepad.buttonState, button);
}


bool isButtonPressed(Input *input, ButtonState button)
{
    return IS_BIT_SET(input->gamepad.buttonState, button) && !IS_BIT_SET(input->gamepad.previousButtonState, button);
}


/*
 *******************************************************************************
 * Video
 *
 * All drawing functions should draw left -> right, top -> bottom.
 *******************************************************************************
 */


void loadSpriteSheet(Video *video, const char *path)
{
    video->loadBMP(path, video->spriteSheet, sizeof(video->spriteSheet) / sizeof(Color32));
}


void loadFont(Video *video, const char *path)
{
    video->loadBMP(path, video->font, sizeof(video->font) / sizeof(Color32));
}


void resetDrawState(Video *video)
{
    video->colorKey = 0x00;
    video->clipRect.top = 0;
    video->clipRect.left = 0;
    video->clipRect.bottom = FRAMEBUFFER_HEIGHT;
    video->clipRect.right = FRAMEBUFFER_WIDTH;
}


void setClippingRect(Video *video, int x, int y, int w, int h)
{
    video->clipRect.left = CLAMP(x, 0, FRAMEBUFFER_WIDTH);
    video->clipRect.right = CLAMP(x + w, 0, FRAMEBUFFER_WIDTH);
    video->clipRect.top = CLAMP(y, 0, FRAMEBUFFER_HEIGHT);
    video->clipRect.bottom = CLAMP(y + h, 0, FRAMEBUFFER_HEIGHT);
}


#define FRAMEBUFFER_POS(x, y)           ((FRAMEBUFFER_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y)    (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)


void clearFramebuffer(Video *video, Color32 color)
{
    Rect clip = video->clipRect;

    for (int i = clip.top; i < clip.bottom; i++)
    {
        for (int j = clip.left; j < clip.right; j++)
            video->framebuffer[FRAMEBUFFER_POS(j, i)] = color;
    }
}


void blitPixel(Video *video, int x, int y, Color32 color)
{
    if (WITHIN_CLIP_RECT(video->clipRect, x, y))
        video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}


static void bresenhamLine(Video *video, int x0, int y0, int x1, int y1, Color32 color)
{
    int xDistance = x1 - x0;
    int yDistance = y1 - y0;
    int xStep = SIGN(xDistance);
    int yStep = SIGN(yDistance);
    xDistance = abs(xDistance) << 1;
    yDistance = abs(xDistance) << 1;
    blitPixel(video, x0, y0, color);

    if (xDistance > yDistance)
    {
        int fraction = yDistance - (xDistance >> 1);
        while (x0 != x1)
        {
            x0 += xStep;
            if (fraction >= 0)
            {
                y0 += yStep;
                fraction -= xDistance;
            }
            fraction += yDistance;
            blitPixel(video, x0, y0, color);
        }
    }
    else
    {
        int fraction = xDistance - (yDistance >> 1);
        while (y0 != y1)
        {
            if (fraction >= 0)
            {
                x0 += xStep;
                fraction -= yDistance;
            }
            y0 += yStep;
            fraction += xDistance;
            blitPixel(video, x0, y0, color);
        }
    }
}


void blitLine(Video *video, int x0, int y0, int x1, int y1, Color32 color)
{
    bresenhamLine(video, x0, y0, x1, y1, color);
}


static void horizontalLine(Video *video, int x, int y, int w, Color32 color)
{
    for (int i = x; i <= x + w; i++)
        blitPixel(video, i, y, color);
}


static void verticalLine(Video *video, int x, int y, int h, Color32 color)
{
    for (int i = y; i <= y + h; i++)
        blitPixel(video, x, i, color);
}


void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
    horizontalLine(video, x, y, w, color); /* Top edge */
    horizontalLine(video, x, y + h, w, color); /* Bottom edge */
    verticalLine(video, x, y, h, color); /* Left edge */
    verticalLine(video, x + w, y, h, color); /* Right edge */
}


void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
    for (int i = y; i < y + h; i++)
    {
        for (int j = x; j < x + w; j++)
            blitPixel(video, j, i, color);
    }
}


#define MIN_SCALE            1
#define MAX_SCALE            5
#define IS_FLIPPED_X(flip)   (flip & 1)
#define IS_FLIPPED_Y(flip)   (flip & 2)


static void nearestNeighbor(Video *video, const Color32 *pixels, int x, int y, int w, int h, int pitch, int scale, u8 flip, const Color32 *color)
{
    scale = CLAMP(scale, MIN_SCALE, MAX_SCALE);

    int width =         w * scale;
    int height =        h * scale;
    int xRatio =        (int) floor((double) (w << 16) / width + 0.5);
    int yRatio =        (int) floor((double) (h << 16) / height + 0.5);
    int xPixelStart =   IS_FLIPPED_X(flip) ? width - 1 : 0;
    int yPixelStart =   IS_FLIPPED_Y(flip) ? height - 1 : 0;
    int xStep =         IS_FLIPPED_X(flip) ? -1 : 1;
    int yStep =         IS_FLIPPED_Y(flip) ? -1 : 1;

    int xPixel, yPixel, xFramebuffer, yFramebuffer;
    for (yFramebuffer = y, yPixel = yPixelStart; yFramebuffer < y + height; yFramebuffer++, yPixel += yStep)
    {
        for (xFramebuffer = x, xPixel = xPixelStart; xFramebuffer < x + width; xFramebuffer++, xPixel += xStep)
        {
            int xNearest = (xPixel * xRatio) >> 16;
            int yNearest = (yPixel * yRatio) >> 16;
            Color32 col = pixels[yNearest * pitch + xNearest];

            if (col != video->colorKey)
                blitPixel(video, xFramebuffer, yFramebuffer, color != NULL ? *color : col);
        }
    }
}


static void blitRect(Video *video, const Color32 *pixels, int x, int y, int w, int h, int pitch, int scale, u8 flip, const Color32 *color)
{
    nearestNeighbor(video, pixels, x, y, w, h, pitch, scale, flip, color);
}


#define SPRSHEET_IDX_OO_BOUNDS(idx)     (idx < 0 || SPRITE_SHEET_SQRSIZE < idx)
#define SPRSHEET_COLUMNS                ((int)(SPRITE_SHEET_SQRSIZE / SPRITE_SQRSIZE))
#define SPRSHEET_ROW_SIZE               ((int)(SPRITE_SHEET_SQRSIZE * SPRITE_SQRSIZE))
#define SPRSHEET_COL_SIZE               SPRITE_SQRSIZE
#define SPRSHEET_POS(x, y)              (y * SPRSHEET_ROW_SIZE + x * SPRSHEET_COL_SIZE)


void blitSprite(Video *video, int idx, int x, int y, int w, int h, int scale, u8 flip)
{
    if (SPRSHEET_IDX_OO_BOUNDS(idx))
        return;

    int width = w * SPRITE_SQRSIZE;
    int height = h * SPRITE_SQRSIZE;
    int row = (int) floor((double) idx / SPRSHEET_COLUMNS);
    int col = (int) floor((double) (idx % SPRSHEET_COLUMNS));
    Color32 *pixels = &video->spriteSheet[SPRSHEET_POS(col, row)];
    blitRect(video, pixels, x, y, width, height, SPRITE_SHEET_SQRSIZE, scale, flip, NULL);
}


#define FONT_COLUMNS         ((int)(FONT_WIDTH / CHAR_WIDTH))
#define FONT_ROW_SIZE        ((int)(FONT_WIDTH * CHAR_HEIGHT))
#define FONT_POS(x, y)       (y * FONT_ROW_SIZE + x * CHAR_WIDTH)
#define IS_ASCII(c)          (0 < c)
#define IS_NEWLINE(c)        (c == '\n')


void blitSpriteFont(Video *video, const Color32 *pixels, int x, int y, const char *str, int scale, Color32 color)
{
    if (str == NULL)
        return;

    int charWidth = (int) floor((double) CHAR_WIDTH * scale);
    int charHeight = (int) floor((double) CHAR_HEIGHT * scale);
    int xCurrent = x;
    int yCurrent = y;
    char curr;

    while ((curr = *str++) != '\0')
    {
        if (!IS_NEWLINE(curr))
        {
            if (!IS_ASCII(curr)) curr = '?';
            int row = (int) floor((double)(curr - 32) / FONT_COLUMNS); /* bitmap font starts at ASCII character 32 (SPACE) */
            int col = (int) floor((double)((curr - 32) % FONT_COLUMNS));
            const Color32 *pixelStart = &pixels[FONT_POS(col, row)];
            blitRect(video, pixelStart, xCurrent, yCurrent, CHAR_WIDTH, CHAR_HEIGHT, FONT_WIDTH, scale, 0, &color);
            xCurrent += charWidth;
        }
        else
        {
            xCurrent = x;
            yCurrent += charHeight;
        }
    }
}


/*
 *******************************************************************************
 * Audio
 *******************************************************************************
 */


#define SAMPLEIDX_OO_BOUNDS(idx)    (idx < 0 || idx > MAX_LOADED_SAMPLES)
#define SLOTIDX_OO_BOUNDS(idx)      (idx < 0 || idx > MAX_SAMPLE_SLOTS)


void resetSampleSlot(SampleSlot *slot)
{
    slot->sampleData = NULL;
    slot->state = STOPPED;
    slot->remainingLength = 0;
    slot->position = NULL;
    slot->volume = 0;
}


void loadSound(Audio *audio, int sampleIdx, const char *filename)
{
    if (SAMPLEIDX_OO_BOUNDS(sampleIdx))
        return;

    audio->lock();

    if (audio->samples[sampleIdx].buffer != NULL)
        unloadSound(audio, sampleIdx);

    audio->loadWAV(audio, filename, sampleIdx);
    audio->unlock();
}


void unloadSound(Audio *audio, int sampleIdx)
{
    if (SAMPLEIDX_OO_BOUNDS(sampleIdx))
        return;

    audio->lock();
    SampleData *sampleData = &audio->samples[sampleIdx];

    if (sampleData != NULL)
    {
        for (int i = 0; i < MAX_SAMPLE_SLOTS; i++)
        {
            if (audio->slots[i].sampleData == sampleData)
                resetSampleSlot(&audio->slots[i]);
        }

        free(audio->samples[sampleIdx].buffer);
        audio->samples[sampleIdx].buffer = NULL;
    }
    audio->unlock();
}


void playSound(Audio *audio, int slotIdx, int sampleIdx, int volume)
{
    if (SAMPLEIDX_OO_BOUNDS(sampleIdx) || SLOTIDX_OO_BOUNDS(slotIdx))
        return;

    SampleData *sampleData = &audio->samples[sampleIdx];

    if (sampleData->length <= 0)
        return;

    audio->lock();
    SampleSlot *slot = &audio->slots[slotIdx];
    slot->sampleData = sampleData;
    slot->state = PLAYING;
    slot->position = sampleData->buffer;
    slot->remainingLength = sampleData->length;
    slot->volume = CLAMP(volume, 0, MAX_VOLUME);
    audio->unlock();
}


void stopSound(Audio *audio, int slotIdx)
{
    if (SLOTIDX_OO_BOUNDS(slotIdx))
        return;

    audio->lock();
    resetSampleSlot(&audio->slots[slotIdx]);
    audio->unlock();
}


void pauseSound(Audio *audio, int slotIdx)
{
    if (SLOTIDX_OO_BOUNDS(slotIdx))
        return;

    if (audio->slots[slotIdx].state == PLAYING)
    {
        audio->lock();
        audio->slots[slotIdx].state = PAUSED;
        audio->unlock();
    }
}


void resumeSound(Audio *audio, int slotIdx)
{
    if (SLOTIDX_OO_BOUNDS(slotIdx))
        return;

    if (audio->slots[slotIdx].state == PAUSED)
    {
        audio->lock();
        audio->slots[slotIdx].state = PLAYING;
        audio->unlock();
    }
}


SampleSlotState getSampleState(Audio *audio, int slotIdx)
{
    return SLOTIDX_OO_BOUNDS(slotIdx) ? STOPPED : audio->slots[slotIdx].state;
}


void setMasterVolume(Audio *audio, int volume)
{
    audio->masterVolume = CLAMP(volume, 0, MAX_VOLUME);
}


#define _16_BIT_MAX 32767


static void mixSample(u8 *destination, const u8 *source, int length, int volume)
{
    i16 sourceSample;
    i16 destSample;
    length /= 2;

    while (length--)
    {
        sourceSample = source[1] << 8 | source[0];
        sourceSample = (sourceSample * volume) / MAX_VOLUME;
        destSample = destination[1] << 8 | destination[0];
        i32 mixedSample = CLAMP(sourceSample + destSample, -_16_BIT_MAX - 1, _16_BIT_MAX);
        destination[0] = mixedSample & 0xff;
        destination[1] = (mixedSample >> 8) & 0xff;
        source += 2;
        destination += 2;
    }
}


#define LOOP_INDEX 0


void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len)
{
    memset(stream, 0, len);

    for (int i = 0; i < MAX_SAMPLE_SLOTS; i++)
    {
        SampleSlot *slot = &audio->slots[i];

        if (slot->sampleData == NULL || slot->state != PLAYING)
            continue;

        if (slot->remainingLength > 0)
        {
            int bytesToWrite = MIN(slot->remainingLength, (int) len);
            mixSample(stream, slot->position, bytesToWrite, slot->volume);
            slot->position += bytesToWrite;
            slot->remainingLength -= bytesToWrite;
        }
        else if (i == LOOP_INDEX)
        {
            slot->position = slot->sampleData->buffer;
            slot->remainingLength = slot->sampleData->length;
        }
        else
        {
            slot->sampleData = NULL;
            slot->state = STOPPED;
        }
    }
}
