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
 * Specifications:
 * - 16 signals loaded in memory at time
 * - 16 concurrently playing signals
 *
 * Signals are dynamically allocated and must be freed.
 * 16 signals can be loaded into memory at a time.
 * To play a signal, it must be inserted into one of the 16 signal slots.
 * One signal can be inserted into multiple signal slots.
 * Signal slot 0 loops.
 *******************************************************************************
 */

#define AUDIO_FREQUENCY     44100
#define AUDIO_CHANNELS      2 /* Stereo */
#define AUDIO_SAMPLES       4096
#define MAX_LOADED_SOUNDS  16
#define MAX_SOUND_SLOTS    16
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

void initAudio(Audio *audio);

void freeAudio(Audio *audio);

void loadSound(Audio *audio, int soundIdx, const char *filename);

void unloadSound(Audio *audio, int soundIdx);

void playSound(Audio *audio, int slotIdx, int soundIdx, int volume);

void stopSound(Audio *audio, int slotIdx);

void pauseSound(Audio *audio, int slotIdx);

void resumeSound(Audio *audio, int slotIdx);

SoundState getSoundState(Audio *audio, int slotIdx);

void setMasterVolume(Audio *audio, int volume);

void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len);

#endif
