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

#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdlib.h>

#include "common.h"

/*
 *******************************************************************************
 * Specification:
 * - [16] sounds loaded in memory at time.
 * - Sounds are played when inserted into one of the [16] sound slots.
 * - A single sound can be inserted into multiple sound slots.
 * - Sound slot [0] loops.
 *
 * Notes:
 * Sounds are dynamically allocated and must be freed.
 * The only memory limit on sound is the hardware.
 *******************************************************************************
 */

#define AUDIO_FREQUENCY     44100
#define AUDIO_CHANNELS      2 /* Stereo */
#define AUDIO_SAMPLES       4096
#define MAX_LOADED_SOUNDS   16
#define MAX_SOUND_SLOTS     16
#define MAX_VOLUME          128

typedef struct soundData
{
    u32 length;
    u8  *samples;
    u8  channelCount;
} SoundData;

typedef enum soundState
{
    STOPPED,
    PLAYING,
    PAUSED
} SoundState;

typedef struct soundSlot
{
    SoundData   *soundData;
    SoundState  state;
    int         volume;
    int         remainingLength;
    u8          *position;
} SoundSlot;

typedef struct audio
{
    SoundData   sounds[MAX_LOADED_SOUNDS];
    SoundSlot   slots[MAX_SOUND_SLOTS];
    int         frequency;
    int         masterVolume;
    int         channels;

    void (*lock)();
    void (*unlock)();
} Audio;

/**
 * Initialize audio.
 * @param audio
 */
void initAudio(Audio *audio);

/**
 * Free audio and all loaded sounds.
 * @param audio
 */
void freeAudio(Audio *audio);

/**
 * Load a sound file's data into the given sound index.
 * @param audio
 * @param soundIdx
 * @param filename
 */
void loadSound(Audio *audio, int soundIdx, const char *filename);

/**
 * Unload the sound data at the given sound index.
 * @param audio
 * @param soundIdx
 */
void unloadSound(Audio *audio, int soundIdx);

/**
 * Play sound[soundIdx] at the given slot index.
 * @param audio
 * @param slotIdx
 * @param soundIdx
 * @param volume
 */
void playSound(Audio *audio, int slotIdx, int soundIdx, int volume);

/**
 * Stop the sound at the given slot index.
 * @param audio
 * @param slotIdx
 */
void stopSound(Audio *audio, int slotIdx);

/**
 * Pause the sound at the given slot index.
 * @param audio
 * @param slotIdx
 */
void pauseSound(Audio *audio, int slotIdx);

/**
 * Resume the sound at the given slot index.
 * @param audio
 * @param slotIdx
 */
void resumeSound(Audio *audio, int slotIdx);

/**
 * Get the state of the given slot index.
 * @param audio
 * @param slotIdx
 * @return
 */
SoundState getSoundState(Audio *audio, int slotIdx);

/**
 * Set the master volume of all sounds.
 * @param audio
 * @param volume
 */
void setMasterVolume(Audio *audio, int volume);

/**
 * Mix all playing sounds into the given stream.
 * @param audio
 * @param stream
 * @param len
 */
void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len);

#endif
