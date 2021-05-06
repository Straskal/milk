#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "wav.h"

// Standard frequency
#define AUDIO_FREQUENCY 44100

// Each sample consists of 16 bits and is represented by a uint16.
#define AUDIO_BITS_PER_SAMPLE 16

// We can only support stereo. Mono sounds are interleaved at runtime.
#define AUDIO_OUTPUT_CHANNELS 2

// We output 4096 samples, which ends up being about 16k bytes.
#define AUDIO_OUTPUT_SAMPLES 4096
#define AUDIO_CHUNK_SIZE (AUDIO_OUTPUT_SAMPLES * (AUDIO_BITS_PER_SAMPLE * AUDIO_OUTPUT_CHANNELS / 8))

// The max number of sounds that can be playing at the same time.
#define MAX_SOUND_SLOTS 16

// The max volume that a sound can be.
#define MAX_VOLUME 128

typedef enum
{
    STOPPED,
    PLAYING,
    PAUSED
} SoundState;

// A sound is assigned to a sound slot by the user.
typedef struct
{
    Wav *soundData;
    SoundState state;
    int volume;
    int remainingSamples;
    int16_t *position;
} SoundSlot;

// Only one stream can be playing at a time.
typedef struct
{
    WavStream *data;
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

// Initialize the audio module.
void initializeAudio(Audio *audio);

// Disable the audio module.
void disableAudio(Audio *audio);

// Play a sound at a specific sound slot.
void playSound(Audio *audio, Wav *wave, int slotId, int volume);

// Stop a sound at a specific sound slot.
void stopSound(Audio *audio, int slotId);

// Stop all instances of a wav sound from playing.
void stopInstances(Audio *audio, Wav *wave);

// Pause the sound at the given slot.
void pauseSound(Audio *audio, int slotId);

// Resume the sound at the given slot.
void resumeSound(Audio *audio, int slotId);

// Get the state of the sound at the given slot.
SoundState getSoundState(Audio *audio, int slotId);

// Play a music stream.
void playStream(Audio *audio, WavStream *waveStream, int volume, bool loop);

// Stop the music stream.
void stopStream(Audio *audio);

// Pause the music stream.
void pauseStream(Audio *audio);

// Resume the music stream.
void resumeStream(Audio *audio);

// Set the master volume. This will update all playing sounds.
void setMasterVolume(Audio *audio, int volume);

// This is more for the platform code to call.
// This function will mix all sounds and music in an audio devices sound stream.
void mixSamplesIntoStream(Audio *audio, int16_t *stream, int numSamples);

#endif
