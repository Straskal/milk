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

#include <string.h>

#include "audio.h"
#include "wav.h"


#define SOUND_IDX_OO_BOUNDS(idx)    (idx < 0 || idx > MAX_LOADED_SOUNDS)
#define SLOT_IDX_OO_BOUNDS(idx)     (idx < 0 || idx > MAX_SOUND_SLOTS)
#define _16_BIT_MAX                 32767
#define LOOP_INDEX                  0


void initAudio(Audio *audio)
{
    memset(&audio->sounds, 0, sizeof(audio->sounds));
    memset(&audio->slots, 0, sizeof(audio->slots));
    audio->masterVolume = MAX_VOLUME;
}


void freeAudio(Audio *audio)
{
    for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
        free(audio->sounds[i].samples);
}


void resetSampleSlot(SoundSlot *slot)
{
    memset(slot, 0, sizeof(SoundSlot));
}


void loadSound(Audio *audio, int soundIdx, const char *filename)
{
    if (SOUND_IDX_OO_BOUNDS(soundIdx))
        return;

    audio->lock();

    SoundData *sound = &audio->sounds[soundIdx];

    if (sound->samples != NULL)
        unloadSound(audio, soundIdx);

    loadWavFile(filename, &sound->samples, &sound->length, &sound->channelCount);

    audio->unlock();
}


void unloadSound(Audio *audio, int soundIdx)
{
    if (SOUND_IDX_OO_BOUNDS(soundIdx))
        return;

    audio->lock();

    SoundData *sound = &audio->sounds[soundIdx];

    if (sound->samples != NULL)
    {
        for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        {
            if (audio->slots[i].soundData == sound)
                resetSampleSlot(&audio->slots[i]);
        }

        free(sound->samples);
        memset(sound, 0, sizeof(SoundData));
    }

    audio->unlock();
}


void playSound(Audio *audio, int slotIdx, int soundIdx, int volume)
{
    if (SOUND_IDX_OO_BOUNDS(soundIdx) || SLOT_IDX_OO_BOUNDS(slotIdx))
        return;

    audio->lock();
    SoundData *sampleData = &audio->sounds[soundIdx];

    if (sampleData->samples != NULL)
    {
        SoundSlot *slot = &audio->slots[slotIdx];
        slot->soundData = sampleData;
        slot->state = PLAYING;
        slot->position = sampleData->samples;
        slot->remainingLength = sampleData->length;
        slot->volume = CLAMP(volume, 0, MAX_VOLUME);
    }

    audio->unlock();
}


void stopSound(Audio *audio, int slotIdx)
{
    audio->lock();
    SoundSlot *slots = audio->slots;

    if (slotIdx == -1)
    {
        for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        {
            if (slots[i].state == PLAYING)
                resetSampleSlot(&slots[i]);
        }
    }
    else if (!SLOT_IDX_OO_BOUNDS(slotIdx))
        resetSampleSlot(&slots[slotIdx]);

    audio->unlock();
}


void pauseSound(Audio *audio, int slotIdx)
{
    audio->lock();
    SoundSlot *slots = audio->slots;

    if (slotIdx == -1)
    {
        for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        {
            if (slots[i].state == PLAYING)
                slots[i].state = PAUSED;
        }
    }
    else if (!SLOT_IDX_OO_BOUNDS(slotIdx))
        slots[slotIdx].state = PAUSED;

    audio->unlock();
}


void resumeSound(Audio *audio, int slotIdx)
{
    audio->lock();
    SoundSlot *slots = audio->slots;

    if (slotIdx == -1)
    {
        for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        {
            if (slots[i].state == PAUSED)
                slots[i].state = PLAYING;
        }
    }
    else if (!SLOT_IDX_OO_BOUNDS(slotIdx))
        slots[slotIdx].state = PLAYING;

    audio->unlock();
}


SoundState getSoundState(Audio *audio, int slotIdx)
{
    return SLOT_IDX_OO_BOUNDS(slotIdx) ? STOPPED : audio->slots[slotIdx].state;
}


void setMasterVolume(Audio *audio, int volume)
{
    audio->masterVolume = CLAMP(volume, 0, MAX_VOLUME);
}


static void mixStereoSamples(u8 *destination, const u8 *source, int length, int volume)
{
    i16 sourceSample;
    i16 destSample;
    i32 mixedSample;
    length /= 2;

    while (length--)
    {
        sourceSample = source[1] << 8 | source[0];
        sourceSample = (sourceSample * volume) / MAX_VOLUME;
        destSample = destination[1] << 8 | destination[0];
        mixedSample = CLAMP(sourceSample + destSample, -_16_BIT_MAX - 1, _16_BIT_MAX);
        destination[0] = mixedSample & 0xff;
        destination[1] = (mixedSample >> 8) & 0xff;
        source += 2;
        destination += 2;
    }
}


static void mixInterleavedMonoSamples(u8 *destination, const u8 *source, int length, int volume)
{
    i16 sourceSample;
    i16 destSample;
    i32 mixedSample;
    length /= 4;

    while (length--)
    {
        sourceSample = source[1] << 8 | source[0];
        sourceSample = (sourceSample * volume) / MAX_VOLUME;
        destSample = destination[1] << 8 | destination[0];
        mixedSample = CLAMP(sourceSample + destSample, -_16_BIT_MAX - 1, _16_BIT_MAX);
        destination[0] = mixedSample & 0xff;
        destination[1] = (mixedSample >> 8) & 0xff;
        destSample = destination[3] << 8 | destination[2];
        mixedSample = CLAMP(sourceSample + destSample, -_16_BIT_MAX - 1, _16_BIT_MAX);
        destination[2] = mixedSample & 0xff;
        destination[3] = (mixedSample >> 8) & 0xff;
        source += 2;
        destination += 4;
    }
}


void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len)
{
    memset(stream, 0, len);

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
    {
        SoundSlot *slot = &audio->slots[i];

        if (slot->soundData == NULL || slot->state != PLAYING)
            continue;

        if (slot->remainingLength > 0)
        {
            int bytesToWrite = MIN(slot->remainingLength, (int) len);

            if (slot->soundData->channelCount == 1)
            {
                mixInterleavedMonoSamples(stream, slot->position, bytesToWrite, slot->volume);
                slot->position += bytesToWrite / 2;
                slot->remainingLength -= bytesToWrite / 2;
            }
            else
            {
                mixStereoSamples(stream, slot->position, bytesToWrite, slot->volume);
                slot->position += bytesToWrite;
                slot->remainingLength -= bytesToWrite;
            }
        }
        else if (i == LOOP_INDEX)
        {
            slot->position = slot->soundData->samples;
            slot->remainingLength = slot->soundData->length;
        }
        else
        {
            slot->soundData = NULL;
            slot->state = STOPPED;
        }
    }
}
