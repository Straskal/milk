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

#endif
