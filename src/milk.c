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
#include "milkapi.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FRAMEBUFFER_POS(x, y) ((MILK_FRAMEBUF_WIDTH * y) + x) /* xy coords to framebuffer pixel index. */
#define WITHIN_CLIP_RECT(clip, x, y) (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)
#define MIN_SCALE 0.5f
#define MAX_SCALE 5.0f

#define FLIPX 1
#define FLIPY 2

/*
 *******************************************************************************
 * Initialization and shutdown
 *******************************************************************************
 */

static void _initVideo(Video *video)
{
	memset(&video->framebuffer, 0x00, sizeof(video->framebuffer));
	memset(&video->spritesheet, 0x00, sizeof(video->spritesheet));
	memset(&video->font,		0x00, sizeof(video->font));
	milkResetDrawState(video);
}

static void _initAudio(Audio *audio)
{
	for (int i = 0; i < MILK_AUDIO_MAX_SOUNDS; i++)
	{
		audio->samples[i].buffer = NULL;
		audio->samples[i].length = 0;
	}

	for (int i = 0; i < MILK_AUDIO_QUEUE_MAX; i++)
	{
		audio->queueItems[i].sampleData = NULL;
		audio->queueItems[i].remainingLength = 0;
		audio->queueItems[i].position = NULL;
		audio->queueItems[i].volume = 0;
		audio->queueItems[i].isLooping = false;
		audio->queueItems[i].isFree = true;
	}

	audio->queue = (AudioQueueItem *)calloc(1, sizeof(AudioQueueItem));
	audio->masterVolume = MILK_AUDIO_MAX_VOLUME;
	audio->frequency = 0;
	audio->channels = 0;
}

Milk *milkCreate()
{
	Milk *milk = (Milk *)calloc(1, sizeof(Milk));
	_initVideo(&milk->video);
	_initAudio(&milk->audio);
	return milk;
}

void milkFree(Milk *milk)
{
	for (int i = 0; i < MILK_AUDIO_MAX_SOUNDS; i++)
		free(milk->audio.samples[i].buffer);

	free(milk->audio.queue);
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

void milkLog(Milk *milk, const char *text, LogType type)
{
	size_t len = strlen(text);

	if (len > MILK_LOG_MAX_LENGTH)
		len = MILK_LOG_MAX_LENGTH;

	if (type == ERROR)
		milk->logs.errorCount++;

	LogMessage *newLogMessage = _getNextFreeLogMessage(&milk->logs);
	strncpy(newLogMessage->text, text, len);
	newLogMessage->length = len;
	newLogMessage->type = type;
}

void milkClearLogs(Milk *milk)
{
	milk->logs.count = 0;
	milk->logs.errorCount = 0;
}

/*
 *******************************************************************************
 * Art
 *******************************************************************************
 */

void milkLoadSpritesheet(Video *video)
{
	video->loadBMP(MILK_SPRSHEET_FILENAME, video->spritesheet, MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SQRSIZE);
}

void milkLoadFont(Video *video)
{
	video->loadBMP(MILK_FONT_FILENAME, video->font, MILK_FONT_WIDTH * MILK_FONT_HEIGHT);
}

/*
 *******************************************************************************
 * Input
 *******************************************************************************
 */

bool milkButton(Input *input, ButtonState button)
{
	return (input->gamepad.buttonState & button) == button;
}

bool milkButtonPressed(Input *input, ButtonState button)
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

void milkResetDrawState(Video *video)
{
	video->colorKey = 0;
	video->clipRect.top = 0;
	video->clipRect.left = 0;
	video->clipRect.bottom = MILK_FRAMEBUF_HEIGHT;
	video->clipRect.right = MILK_FRAMEBUF_WIDTH;
}

static int _clamp(int value, int min, int max)
{
	if (value < min)
		value = min;

	if (value > max)
		value = max;

	return value;
}

void milkClipRect(Video *video, int x, int y, int w, int h)
{
	video->clipRect.left = _clamp(x, 0, MILK_FRAMEBUF_WIDTH);
	video->clipRect.right = _clamp(x + w, 0, MILK_FRAMEBUF_WIDTH);
	video->clipRect.top = _clamp(y, 0, MILK_FRAMEBUF_HEIGHT);
	video->clipRect.bottom = _clamp(y + h, 0, MILK_FRAMEBUF_HEIGHT);
}

void milkClear(Video *video, Color32 color)
{
	Rect clip = video->clipRect;

	for (int i = clip.top; i < clip.bottom; i++)
	{
		for (int j = clip.left; j < clip.right; j++)
			video->framebuffer[FRAMEBUFFER_POS(j, i)] = color;
	}
}

void milkPixelSet(Video *video, int x, int y, Color32 color)
{
	if (WITHIN_CLIP_RECT(video->clipRect, x, y))
		video->framebuffer[FRAMEBUFFER_POS(x, y)] = color;
}

static void _horizontalLine(Video *video, int x, int y, int w, Color32 color)
{
	for (int i = x; i <= x + w; i++)
		milkPixelSet(video, i, y, color);
}

static void _verticalLine(Video *video, int x, int y, int h, Color32 color)
{
	for (int i = y; i <= y + h; i++)
		milkPixelSet(video, x, i, color);
}

void milkRect(Video *video, int x, int y, int w, int h, Color32 color)
{
	_horizontalLine(video, x, y, w, color);
	_horizontalLine(video, x, y + h, w, color);
	_verticalLine(video, x, y, h, color);
	_verticalLine(video, x + w, y, h, color);
}

void milkRectFill(Video *video, int x, int y, int w, int h, Color32 color)
{
	for (int i = y; i < y + h; i++)
	{
		for (int j = x; j < x + w; j++)
			milkPixelSet(video, j, i, color);
	}
}

/*
 * Main helper function to blit pixel images onto the framebuffer.
 * We're pretty much running nearest neighbor scaling on all blit pixels.
 * This greatly simplifies the code, so it should stay this way unless it starts causing performance issues.
 *
 * "Nearest neighbor scaling replaces every pixel with the nearest pixel in the output.
 *  When upscaling an image, multiple pixels of the same color will be duplicated throughout the image." - Some random explanation on google.
 */
static void _blitRect(Video *video, Color32 *pixels, int x, int y, int w, int h, int pitch, float scale, int flip, Color32 *color)
{
	if (scale <= MIN_SCALE)
		scale = MIN_SCALE;

	if (scale > MAX_SCALE)
		scale = MAX_SCALE;

	int width = (int)floor((double)w * scale);
	int height = (int)floor((double)h * scale);
	int xRatio = (int)((w << 16) / width) + 1;
	int yRatio = (int)((h << 16) / height) + 1;
	int xflip = (flip & FLIPX) == FLIPX;
	int yflip = (flip & FLIPY) == FLIPY;
	int xPixelStart = xflip ? width - 1 : 0;
	int xDirection = xflip ? -1 : 1;
	int yPixelStart = yflip ? height - 1 : 0;
	int yDirection = yflip ? -1 : 1;
	int xPixel, yPixel;
	int xFramebuffer, yFramebuffer;

	for (yFramebuffer = y, yPixel = yPixelStart; yFramebuffer < y + height; yFramebuffer++, yPixel += yDirection)
	{
		for (xFramebuffer = x, xPixel = xPixelStart; xFramebuffer < x + width; xFramebuffer++, xPixel += xDirection)
		{
			int xNearest = (xPixel * xRatio) >> 16;
			int yNearest = (yPixel * yRatio) >> 16;
			Color32 col = pixels[yNearest * pitch + xNearest];

			if (col != video->colorKey)
				milkPixelSet(video, xFramebuffer, yFramebuffer, color != NULL ? *color : col);
		}
	}
}

void milkSprite(Video *video, int idx, int x, int y, int w, int h, float scale, int flip)
{
	const int numColumns = MILK_SPRSHEET_SQRSIZE / MILK_SPRSHEET_SPR_SQRSIZE;
	const int rowSize = MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SPR_SQRSIZE;
	const int colSize = MILK_SPRSHEET_SPR_SQRSIZE;

	if (idx < 0 || MILK_SPRSHEET_SQRSIZE < idx)
		return;

	int row = (int)floor(idx / numColumns);
	int col = (int)floor(idx % numColumns);
	Color32 *pixels = &video->spritesheet[row * rowSize + col * colSize];

	_blitRect(video, pixels, x, y, w * MILK_SPRSHEET_SPR_SQRSIZE, h * MILK_SPRSHEET_SPR_SQRSIZE, MILK_SPRSHEET_SQRSIZE, scale, flip, NULL);
}

void milkSpriteFont(Video *video, int x, int y, const char *str, float scale, Color32 color)
{
	#define IS_ASCII(c)		((c & 0xff80) == 0)
	#define IS_NEWLINE(c)	(c == '\n')

	const int numColumns = MILK_FONT_WIDTH / MILK_CHAR_SQRSIZE;
	const int rowSize = MILK_FONT_WIDTH * MILK_CHAR_SQRSIZE;
	const int colSize = MILK_CHAR_SQRSIZE;

	if (str == NULL)
		return;

	int charSize = (int)floor((double)MILK_CHAR_SQRSIZE * scale);
	int xCurrent = x;
	int yCurrent = y;

	while (*str)
	{
		if (!IS_NEWLINE(*str))
		{
			char ch = *(str++);
			if (!IS_ASCII(ch)) ch = '?'; /* If the character is not ASCII, then we're just gonna be all like whaaaaaat? Problem solved. */
			int row = (int)floor((ch - 32) / numColumns); /* bitmap font starts at ASCII character 32 (SPACE) */
			int col = (int)floor((ch - 32) % numColumns);
			Color32 *pixels = &video->font[(row * rowSize + col * colSize)];
			_blitRect(video, pixels, xCurrent, yCurrent, MILK_CHAR_SQRSIZE, MILK_CHAR_SQRSIZE, MILK_FONT_WIDTH, scale, 0, &color);
			xCurrent += charSize;
		}
		else
		{
			xCurrent = x;
			yCurrent += charSize;
			str++;
		}
	}

	#undef IS_ASCII
	#undef IS_NEWLINE
}

/*
 *******************************************************************************
 * Audio
 *******************************************************************************
 */

static void _removeSampleFromQueue(AudioQueueItem *queue, SampleData *sampleData)
{
	AudioQueueItem *curr = queue->next;
	AudioQueueItem *prev = queue;

	while (curr != NULL)
	{
		if (curr->sampleData == sampleData)
		{
			curr->isFree = true;
			prev->next = curr->next;
		}

		prev = curr;
		curr = curr->next;
	}
}

void milkLoadSound(Audio *audio, int idx, const char *filename)
{
	if (idx < 0 || idx > MILK_AUDIO_MAX_SOUNDS)
		return;

	audio->lock();

	/* If we're loading a sound into a sample data that in use, then remove all instances playing in the queue, and then free it. */
	if (audio->samples[idx].buffer != NULL)
	{
		_removeSampleFromQueue(audio->queue, &audio->samples[idx]);
		free(audio->samples[idx].buffer);
	}

	audio->loadWAV(audio, filename, idx);
	audio->unlock();
}

static bool _getFreeQueueItem(Audio *audio, AudioQueueItem **queueItem)
{
	for (int i = 0; i < MILK_AUDIO_QUEUE_MAX; i++)
	{
		if (audio->queueItems[i].isFree)
		{
			audio->queueItems[i].isFree = false; /* Queue item is not free any more. */
			*queueItem = &audio->queueItems[i];
			return true;
		}
	}
	return false;
}

static void _removeLoopingSampleFromQueue(Audio *audio)
{
	AudioQueueItem *curr = audio->queue->next;
	AudioQueueItem *prev = audio->queue;

	while (curr != NULL)
	{
		if (curr->isLooping)
		{
			curr->isFree = true;
			prev->next = curr->next;
			break;
		}

		prev = curr;
		curr = curr->next;
	}
}

static void _queueSample(Audio *audio, AudioQueueItem *new)
{
	AudioQueueItem *curr = audio->queue;

	while (curr->next != NULL)
		curr = curr->next;

	curr->next = new;
}

void milkSound(Audio *audio, int idx, int volume, bool loop)
{
	if (idx < 0 || idx > MILK_AUDIO_MAX_SOUNDS)
		return;

	SampleData *sampleData = &audio->samples[idx];
	if (sampleData->length == 0)
		return;

	audio->lock();

	AudioQueueItem *queueItem;
	if (_getFreeQueueItem(audio, &queueItem))
	{
		if (loop) /* Stop current loop in favor of the new looping sound. */
			_removeLoopingSampleFromQueue(audio);

		queueItem->sampleData = sampleData;
		queueItem->position = sampleData->buffer;
		queueItem->remainingLength = sampleData->length;
		queueItem->volume = (uint8_t)_clamp(volume, 0, MILK_AUDIO_MAX_VOLUME);
		queueItem->isLooping = loop;
		queueItem->next = NULL;

		_queueSample(audio, queueItem);
	}
	audio->unlock();
}

void milkVolume(Audio *audio, int volume)
{
	audio->masterVolume = (uint8_t)_clamp(volume, 0, MILK_AUDIO_MAX_VOLUME);
}

static void _mixSample(uint8_t *destination, uint8_t *source, uint32_t length, double volume)
{
	#define _16_BIT_MAX 32767

	int16_t sourceLeft;
	int16_t sourceRight;
	length /= 2;

	while (length--)
	{
		sourceLeft = (int16_t)((source[1] << 8 | source[0]) * volume);
		sourceRight = (int16_t)((destination[1] << 8 | destination[0]));
		int mixedSample = sourceLeft + sourceRight;

		if (mixedSample > _16_BIT_MAX)
			mixedSample = _16_BIT_MAX;
		else if (mixedSample < -_16_BIT_MAX - 1)
			mixedSample = -_16_BIT_MAX - 1;

		destination[0] = mixedSample & 0xff;
		destination[1] = (mixedSample >> 8) & 0xff;
		source += 2;
		destination += 2;
	}

	#undef _16_BIT_MAX
}

void milkMixCallback(void *userdata, uint8_t *stream, int len)
{
	#define NORMALIZE_VOLUME(v) (double)(v / MILK_AUDIO_MAX_VOLUME)

	memset(stream, 0, len);

	Audio *audio = (Audio *)userdata;
	AudioQueueItem *currentItem = audio->queue->next;
	AudioQueueItem *previousItem = audio->queue;

	while (currentItem != NULL)
	{
		if (currentItem->remainingLength > 0) /* If the queue item still has remaining samples to spend, then mix it and update its length. */
		{
			uint32_t bytesToWrite = ((uint32_t)len > currentItem->remainingLength) ? currentItem->remainingLength : (uint32_t)len;
			_mixSample(stream, currentItem->position, bytesToWrite, NORMALIZE_VOLUME(currentItem->volume));
			currentItem->position += bytesToWrite;
			currentItem->remainingLength -= bytesToWrite;
			previousItem = currentItem;
			currentItem = currentItem->next;
		}
		else if (currentItem->isLooping) /* Else if the sound loops (music), then reset it's buffer position and length to the beginning. */
		{
			currentItem->position = currentItem->sampleData->buffer;
			currentItem->remainingLength = currentItem->sampleData->length;
		}
		else /* Else the sound is completely finished and can be removed from the queue, freeing up space for another sound. */
		{
			AudioQueueItem *next = currentItem->next;
			currentItem->isFree = true;
			previousItem->next = next;
			currentItem->next = NULL;
			currentItem = next;
		}
	}

	#undef NORMALIZE_VOLUME
}
