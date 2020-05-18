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

#ifndef __MILK_AUDIO_H__
#define __MILK_AUDIO_H__

#include <stdint.h>

#define MILK_AUDIO_FREQUENCY 44100
#define MILK_AUDIO_CHANNELS 2 /* Stereo */
#define MILK_AUDIO_SAMPLES 4096
#define MILK_AUDIO_MAX_SOUNDS 25
#define MILK_AUDIO_QUEUE_MAX 16
#define MILK_AUDIO_MAX_VOLUME 128

typedef struct
{
    uint32_t length;
    uint8_t *buffer;
} SampleData;

typedef struct
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

void milkOpenAudio(Audio *audio);
void milkCloseAudio(Audio *audio);
void milkPlayMusic(Audio *audio, int idx, uint8_t volume);
void milkSound(Audio *audio, int idx, uint8_t volume);
void milkVolume(Audio *audio, uint8_t volume);
void milkMixCallback(void *userdata, uint8_t *stream, int len);

#endif
