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

#include "milk.h"
#include "api.h"
#include "embed/font.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 *******************************************************************************
 * Helpers
 *******************************************************************************
 */


static int _clamp(int value, int min, int max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;

	return value;
}


static float _clampf(float value, float min, float max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;

	return value;
}

/*
 *******************************************************************************
 * Initialization and shutdown
 *******************************************************************************
 */


static void _initLogs(Logs *logs)
{
	for (int i = 0; i < MILK_MAX_LOGS; i++)
	{
		logs->messages[i].length = 0;
		logs->messages[i].type = 0;
		memset(logs->messages[i].text, 0, MILK_LOG_MAX_LENGTH);
	}

	logs->count = 0;
	logs->errorCount = 0;
}


static void _initInput(Input *input)
{
	input->gamepad.buttonState = 0;
	input->gamepad.previousButtonState = 0;
}


static void _initVideo(Video *video)
{
	memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
	memset(&video->spritesheet, 0x00, sizeof(video->spritesheet));
	memcpy(&video->font, DEFAULT_FONT_DATA, sizeof(video->font));
	resetDrawState(video);
}


static void _initAudio(Audio *audio)
{
	for (int i = 0; i < MILK_MAX_LOADED_SAMPLES; i++)
	{
		audio->samples[i].buffer = NULL;
		audio->samples[i].length = 0;
	}

	for (int i = 0; i < MILK_MAX_CONCUR_SOUNDS; i++)
	{
		audio->slots[i].sampleData = NULL;
		audio->slots[i].state = STOPPED;
		audio->slots[i].remainingLength = 0;
		audio->slots[i].position = NULL;
		audio->slots[i].volume = 0;
	}

	audio->masterVolume = MILK_AUDIO_MAX_VOLUME;
	audio->frequency = 0;
	audio->channels = 0;
}


static void _initCode(Code *code)
{
	code->state = NULL;
}


Milk *createMilk()
{
	Milk *milk = (Milk *)malloc(sizeof(Milk));
	milk->shouldQuit = false;

	_initLogs(&milk->logs);
	_initInput(&milk->input);
	_initVideo(&milk->video);
	_initAudio(&milk->audio);
	_initCode(&milk->code);

	return milk;
}


void freeMilk(Milk *milk)
{
	for (int i = 0; i < MILK_MAX_LOADED_SAMPLES; i++)
		free(milk->audio.samples[i].buffer);

	free(milk);
}


/*
 *******************************************************************************
 * Logging
 *******************************************************************************
 */


 /* When milk's log array is full, we shift down all of the logs before inserting the next one.*/
static LogMessage *_getNextFreeLogMessage(Logs *logs)
{
	if (logs->count == MILK_MAX_LOGS)
	{
		for (int i = 0; i < MILK_MAX_LOGS - 1; i++)
			logs->messages[i] = logs->messages[i + 1];

		return &logs->messages[MILK_MAX_LOGS - 1];
	}
	else
		return &logs->messages[logs->count++];
}


void logMessage(Logs *logs, const char *text, LogType type)
{
	size_t len = strlen(text);

	if (len > MILK_LOG_MAX_LENGTH)
		len = MILK_LOG_MAX_LENGTH;

	if (type == ERROR)
		logs->errorCount++;

	LogMessage *newLogMessage = _getNextFreeLogMessage(logs);
	memset(newLogMessage->text, 0, MILK_LOG_MAX_LENGTH);
	strncpy(newLogMessage->text, text, len);
	newLogMessage->length = len;
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


bool isButtonDown(Input *input, ButtonState button)
{
	return (input->gamepad.buttonState & button) == button;
}


bool isButtonPressed(Input *input, ButtonState button)
{
	return (input->gamepad.buttonState & button) == button && (input->gamepad.previousButtonState & button) != button;
}


/*
 *******************************************************************************
 * Video
 *
 * All drawing functions should draw left -> right, top -> bottom.
 *******************************************************************************
 */


void loadSpritesheet(Video *video, const char *path)
{
	video->loadBMP(path, video->spritesheet, sizeof(video->spritesheet) / sizeof(Color32));
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
	video->clipRect.bottom = MILK_FRAMEBUF_HEIGHT;
	video->clipRect.right = MILK_FRAMEBUF_WIDTH;
}


void setClippingRect(Video *video, int x, int y, int w, int h)
{
	video->clipRect.left =		_clamp(x,		0, MILK_FRAMEBUF_WIDTH);
	video->clipRect.right =		_clamp(x + w,	0, MILK_FRAMEBUF_WIDTH);
	video->clipRect.top =		_clamp(y,		0, MILK_FRAMEBUF_HEIGHT);
	video->clipRect.bottom =	_clamp(y + h,	0, MILK_FRAMEBUF_HEIGHT);
}


#define FRAMEBUFFER_POS(x, y)			((MILK_FRAMEBUF_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y)	(clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)


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


static void _horizontalLine(Video *video, int x, int y, int w, Color32 color)
{
	for (int i = x; i <= x + w; i++)
		blitPixel(video, i, y, color);
}


static void _verticalLine(Video *video, int x, int y, int h, Color32 color)
{
	for (int i = y; i <= y + h; i++)
		blitPixel(video, x, i, color);
}


void blitRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
	_horizontalLine	(video, x,		y,		w,		color); /* Top edge */
	_horizontalLine	(video, x,		y + h,	w,		color); /* Bottom edge */
	_verticalLine	(video, x,		y,		h,		color); /* Left edge */
	_verticalLine	(video, x + w,	y,		h,		color); /* Right edge */
}


void blitFilledRectangle(Video *video, int x, int y, int w, int h, Color32 color)
{
	for (int i = y; i < y + h; i++)
	{
		for (int j = x; j < x + w; j++)
			blitPixel(video, j, i, color);
	}
}


#define MIN_SCALE			0.5f
#define MAX_SCALE			5.0f
#define IS_FLIPPED_X(flip)	((flip & 1) == 1)
#define IS_FLIPPED_Y(flip)	((flip & 2) == 2)


/*
 * Main helper function to blit pixel images onto the framebuffer.
 * We're pretty much running nearest neighbor scaling on all blit pixels.
 * This greatly simplifies the code, so it should stay this way unless it starts causing performance issues.
 *
 * "Nearest neighbor scaling replaces every pixel with the nearest pixel in the output.
 *  When upscaling an image, multiple pixels of the same color will be duplicated throughout the image." - Some random explanation on google.
 */
static void _blitRect(Video *video, const Color32 *pixels, int x, int y, int w, int h, int pitch, float scale, int flip, Color32 *color)
{
	scale =	_clampf(scale, MIN_SCALE, MAX_SCALE);

	int width =		(int)floor((double)w * scale);
	int height =	(int)floor((double)h * scale);
	int xRatio =	(int)(((w << 16) / width) + 0.5f);
	int yRatio =	(int)(((h << 16) / height) + 0.5f);

	int xPixelStart =	IS_FLIPPED_X(flip) ? width - 1 : 0;
	int yPixelStart =	IS_FLIPPED_Y(flip) ? height - 1 : 0;
	int xDirection =	IS_FLIPPED_X(flip) ? -1 : 1;
	int yDirection =	IS_FLIPPED_Y(flip) ? -1 : 1;

	int xPixel, yPixel, xFramebuffer, yFramebuffer;

	for (yFramebuffer = y, yPixel = yPixelStart; yFramebuffer < y + height; yFramebuffer++, yPixel += yDirection)
	{
		for (xFramebuffer = x, xPixel = xPixelStart; xFramebuffer < x + width; xFramebuffer++, xPixel += xDirection)
		{
			int xNearest = (xPixel * xRatio) >> 16;
			int yNearest = (yPixel * yRatio) >> 16;
			Color32 col = pixels[yNearest * pitch + xNearest];

			if (col != video->colorKey)
				blitPixel(video, xFramebuffer, yFramebuffer, color != NULL ? *color : col);
		}
	}
}


#define SPRSHEET_IDX_OO_BOUNDS(idx)	(idx < 0 || MILK_SPRSHEET_SQRSIZE < idx)
#define SPRSHEET_COLUMNS			MILK_SPRSHEET_SQRSIZE / MILK_SPRSHEET_SPR_SQRSIZE
#define SPRSHEET_ROW_SIZE			MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SPR_SQRSIZE
#define SPRSHEET_COL_SIZE			MILK_SPRSHEET_SPR_SQRSIZE
#define SPRSHEET_POS(x, y)			(y * SPRSHEET_ROW_SIZE + x * SPRSHEET_COL_SIZE)


void blitSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int flip)
{
	if (SPRSHEET_IDX_OO_BOUNDS(idx))
		return;

	int row = (int)floor(idx / SPRSHEET_COLUMNS);
	int col = (int)floor(idx % SPRSHEET_COLUMNS);
	Color32 *pixels = &video->spritesheet[SPRSHEET_POS(col, row)];

	_blitRect(video, pixels, x, y, w * MILK_SPRSHEET_SPR_SQRSIZE, h * MILK_SPRSHEET_SPR_SQRSIZE, MILK_SPRSHEET_SQRSIZE, scale, flip, NULL);
}


#define FONT_COLUMNS		(MILK_FONT_WIDTH / MILK_CHAR_SQRSIZE)
#define FONT_ROW_SIZE		(MILK_FONT_WIDTH * MILK_CHAR_SQRSIZE)
#define FONT_COL_SIZE		MILK_CHAR_SQRSIZE
#define FONT_POS(x, y)		(y * FONT_ROW_SIZE + x * FONT_COL_SIZE)
#define IS_ASCII(c)			((c & 0xff80) == 0)
#define IS_NEWLINE(c)		(c == '\n')


void blitSpritefont(Video *video, const Color32 *pixels, int x, int y, const char *str, float scale, Color32 color)
{
	if (str == NULL)
		return;

	int charSize = (int)floor((double)MILK_CHAR_SQRSIZE * scale);
	int xCurrent = x;
	int yCurrent = y;
	char curr;

	while ((curr = *str++) != '\0')
	{
		if (!IS_NEWLINE(curr))
		{
			if (!IS_ASCII(curr)) curr = '?'; /* If the character is not ASCII, then we're just gonna be all like whaaaaaat? Problem solved. */
			int row = (int)floor((curr - 32) / FONT_COLUMNS); /* bitmap font starts at ASCII character 32 (SPACE) */
			int col = (int)floor((curr - 32) % FONT_COLUMNS);
			const Color32 *pixelStart = &pixels[FONT_POS(col, row)];

			_blitRect(video, pixelStart, xCurrent, yCurrent, MILK_CHAR_SQRSIZE, MILK_CHAR_SQRSIZE, MILK_FONT_WIDTH, scale, 0, &color);
			xCurrent += charSize;
		}
		else
		{
			xCurrent = x;
			yCurrent += charSize;
		}
	}
}


/*
 *******************************************************************************
 * Audio
 *******************************************************************************
 */


#define SAMPLEIDX_OO_BOUNDS(idx)	(idx < 0 || idx > MILK_MAX_LOADED_SAMPLES)
#define SLOTIDX_OO_BOUNDS(idx)		(idx < 0 || idx > MILK_MAX_CONCUR_SOUNDS)


void _resetSampleSlot(SampleSlot *slot)
{
	slot->sampleData = NULL;
	slot->state = STOPPED;
	slot->remainingLength = 0;
	slot->position = NULL;
	slot->volume = 0;
}


void loadSound(Audio *audio, int idx, const char *filename)
{
	if (SAMPLEIDX_OO_BOUNDS(idx))
		return;

	audio->lock();

	if (audio->samples[idx].buffer != NULL)
		unloadSound(audio, idx);

	audio->loadWAV(audio, filename, idx);
	audio->unlock();
}


void unloadSound(Audio *audio, int idx)
{
	if (SAMPLEIDX_OO_BOUNDS(idx))
		return;

	audio->lock();
	SampleData *sampleData = &audio->samples[idx];

	if (sampleData != NULL)
	{
		for (int i = 0; i < MILK_MAX_CONCUR_SOUNDS; i++)
		{
			if (audio->slots[i].sampleData == sampleData)
				_resetSampleSlot(&audio->slots[i]);
		}

		free(audio->samples[idx].buffer);
		audio->samples[idx].buffer = NULL;
	}
	audio->unlock();
}


void playSound(Audio *audio, int sampleIdx, int slotIdx, int volume)
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
	slot->volume = (uint8_t)_clamp(volume, 0, MILK_AUDIO_MAX_VOLUME);
	audio->unlock();
}


void stopSound(Audio *audio, int slotIdx)
{
	if (SLOTIDX_OO_BOUNDS(slotIdx))
		return;

	audio->lock();
	_resetSampleSlot(&audio->slots[slotIdx]);
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
	audio->masterVolume = (uint8_t)_clamp(volume, 0, MILK_AUDIO_MAX_VOLUME);
}


#define _16_BIT_MAX 32767


static void _mixSample(uint8_t *destination, uint8_t *source, uint32_t length, double volume)
{
	int16_t sourceSample;
	int16_t destSample;
	length /= 2;

	while (length--)
	{
		sourceSample = (int16_t)((source[1] << 8 | source[0]) * volume);
		destSample = (int16_t)((destination[1] << 8 | destination[0]));
		int mixedSample = _clamp(sourceSample + destSample, -_16_BIT_MAX - 1, _16_BIT_MAX);
		destination[0] = mixedSample & 0xff;
		destination[1] = (mixedSample >> 8) & 0xff;
		source += 2;
		destination += 2;
	}
}


#define LOOP_INDEX			0
#define NORMALIZE_VOLUME(v) (double)(v / MILK_AUDIO_MAX_VOLUME)


void mixSamplesIntoStream(Audio *audio, uint8_t *stream, int len)
{
	memset(stream, 0, len);

	for (int i = 0; i < MILK_MAX_CONCUR_SOUNDS; i++)
	{
		SampleSlot *slot = &audio->slots[i];

		if (slot->sampleData == NULL || slot->state != PLAYING)
			continue;

		if (slot->remainingLength > 0)
		{
			uint32_t bytesToWrite = ((uint32_t)len > slot->remainingLength) ? slot->remainingLength : (uint32_t)len;
			_mixSample(stream, slot->position, bytesToWrite, NORMALIZE_VOLUME(slot->volume));
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
