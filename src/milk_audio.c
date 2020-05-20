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

#include "milk_audio.h"

#include <stdlib.h>
#include <SDL.h>

static void _loadWav(const char *filename, SampleData *sampleData)
{
	SDL_AudioSpec waveSpec;
	SDL_LoadWAV(filename, &waveSpec, &sampleData->buffer, &sampleData->length);

	/* A lot of sound effects are mono (1 channel), so must CONVERT THEM. */
	if (waveSpec.channels != MILK_AUDIO_CHANNELS)
	{
		SDL_AudioCVT conversion;
		SDL_BuildAudioCVT(&conversion, waveSpec.format, waveSpec.channels, waveSpec.freq, AUDIO_S16LSB, MILK_AUDIO_CHANNELS, MILK_AUDIO_FREQUENCY);
		conversion.len = sampleData->length;
		conversion.buf = (Uint8 *)SDL_malloc((size_t)conversion.len * conversion.len_mult);
		SDL_memcpy(conversion.buf, sampleData->buffer, sampleData->length);
		SDL_ConvertAudio(&conversion);
		SDL_FreeWAV(sampleData->buffer);

		sampleData->buffer = conversion.buf;
		sampleData->length = conversion.len * conversion.len_ratio;;
	}
}

void milkOpenAudio(Audio *audio)
{
	audio->queue = (AudioQueueItem *)calloc(1, sizeof(AudioQueueItem));
	audio->masterVolume = MILK_AUDIO_MAX_VOLUME;

	for (int i = 0; i < MILK_AUDIO_QUEUE_MAX; i++)
		audio->queueItems[i].isFree = 1;

	_loadWav("music.wav", &audio->samples[0]);
	_loadWav("fireball_shoot.wav", &audio->samples[1]);
	_loadWav("punch.wav", &audio->samples[2]);
}

void milkCloseAudio(Audio *audio)
{
	free(audio->queue);

	for (int i = 0; i < MILK_AUDIO_MAX_SOUNDS; i++)
		SDL_FreeWAV(audio->samples[i].buffer);
}

static void _queueSample(AudioQueueItem *queue, AudioQueueItem *new)
{
	while (queue->next != NULL)
		queue = queue->next;

	queue->next = new;
}

/* Milk only allows for 1 looping sound at a time. Stop loop must be called before mixing another looping sound. */
static void _stopCurrentLoop(AudioQueueItem *queue)
{
	AudioQueueItem *curr = queue->next;
	AudioQueueItem *prev = queue;

	while (curr != NULL)
	{
		if (curr->loop)
		{
			curr->isFree = 1;
			prev->next = curr->next;
			break;
		}

		prev = curr;
		curr = curr->next;
	}
}

/* Milk limits the amount of concurrent sounds. If a free queue spot isn't found, then the sound isn't played. END OF FUCKING STORY, BABY. */
static int _getFreeQueueItem(Audio *audio, AudioQueueItem **queueItem)
{
	for (int i = 0; i < MILK_AUDIO_QUEUE_MAX; i++)
	{
		if (audio->queueItems[i].isFree)
		{
			audio->queueItems[i].isFree = 0; /* Queue item is not free any more. */
			*queueItem = &audio->queueItems[i];
			return 1;
		}
	}
	return 0;
}

void milkSound(Audio *audio, int idx, uint8_t volume, uint8_t loop)
{
	audio->lock();
	AudioQueueItem *queueItem;

	if (_getFreeQueueItem(audio, &queueItem))
	{
		if (loop) /* Stop current loop in favor of the new looping sound. */
			_stopCurrentLoop(audio->queue);

		SampleData *sampleData = &audio->samples[idx];
		queueItem->sampleData = sampleData;
		queueItem->position = sampleData->buffer;
		queueItem->remainingLength = sampleData->length;
		queueItem->volume = volume;
		queueItem->loop = loop;
		queueItem->next = NULL;

		_queueSample(audio->queue, queueItem);
	}
	audio->unlock();
}

void milkVolume(Audio *audio, uint8_t volume)
{
	if (volume < 0)
		volume = 0;
	if (volume > MILK_AUDIO_MAX_VOLUME)
		volume = MILK_AUDIO_MAX_VOLUME;

	audio->masterVolume = volume;
}

/*
 * Mix milk's audio queue samples in the audio device's sample stream.
 * Milk limits the queue size to 16 sounds at a given time, so many mixed sounds should not cause distortion.
 */
void milkMixCallback(void *userdata, uint8_t *stream, int len)
{
	Audio *audio = (Audio *)userdata;
	AudioQueueItem *currentItem = audio->queue->next;
	AudioQueueItem *previousItem = audio->queue;
	SDL_memset(stream, 0, len); /* Silence the stream before writing to it. */

	while (currentItem != NULL)
	{
		if (currentItem->remainingLength > 0)
		{
			uint32_t bytesToWrite = ((uint32_t)len > currentItem->remainingLength) ? currentItem->remainingLength : (uint32_t)len;
			double volNormalized = ((double)currentItem->volume / MILK_AUDIO_MAX_VOLUME);

			SDL_MixAudioFormat(stream, currentItem->position, AUDIO_S16LSB, bytesToWrite, (int)round(volNormalized * audio->masterVolume));

			currentItem->position += bytesToWrite;
			currentItem->remainingLength -= bytesToWrite;
			previousItem = currentItem;
			currentItem = currentItem->next;
		}
		else if (currentItem->loop)
		{
			/* Music loops. */
			currentItem->position = currentItem->sampleData->buffer;
			currentItem->remainingLength = currentItem->sampleData->length;
		}
		else
		{
			AudioQueueItem *next = currentItem->next;
			currentItem->isFree = 1;
			previousItem->next = next;
			currentItem->next = NULL;
			currentItem = next;
		}
	}
}
