#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "wave.h"

#define AUDIO_FREQUENCY 44100
#define AUDIO_BITS_PER_SAMPLE 16
#define AUDIO_OUTPUT_CHANNELS 2
#define AUDIO_OUTPUT_SAMPLES 4096
#define AUDIO_CHUNK_SIZE (AUDIO_OUTPUT_SAMPLES * (AUDIO_BITS_PER_SAMPLE * AUDIO_OUTPUT_CHANNELS / 8))
#define MAX_SOUND_SLOTS 16
#define MAX_VOLUME 128

typedef enum
{
    STOPPED,
    PLAYING,
    PAUSED
} SoundState;

typedef struct
{
    Wave *soundData;
    SoundState state;
    int volume;
    int remainingSamples;
    int16_t *position;
} SoundSlot;

typedef struct
{
    WaveStream *data;
    SoundState state;
    int volume;
    bool loop;
} StreamSlot;

typedef struct
{
    SoundSlot soundSlots[MAX_SOUND_SLOTS];
    StreamSlot streamSlot;
    int masterVolume;
} Audio;

void initializeAudio(Audio *audio);
void disableAudio(Audio *audio);
void playSound(Audio *audio, Wave *wave, int slotId, int volume);
void stopSound(Audio *audio, int slotId);
void stopInstances(Audio *audio, Wave *wave);
void pauseSound(Audio *audio, int slotId);
void resumeSound(Audio *audio, int slotId);
SoundState getSoundState(Audio *audio, int slotId);
void playStream(Audio *audio, WaveStream *waveStream, int volume, bool loop);
void stopStream(Audio *audio);
void pauseStream(Audio *audio);
void resumeStream(Audio *audio);
void setMasterVolume(Audio *audio, int volume);
void mixSamplesIntoStream(Audio *audio, int16_t *stream, int numSamples);

#endif
