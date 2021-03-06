#include <stdbool.h>
#include <string.h>

#include "audio.h"
#include "common.h"
#include "platform.h"

#define S16_MAX 32767
#define S16_MIN -32768

void initializeAudio(Audio *audio)
{
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(&audio->streamSlot, 0, sizeof(audio->streamSlot));
  audio->masterVolume = MAX_VOLUME;
}

void disableAudio(Audio *audio)
{
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(&audio->streamSlot, 0, sizeof(audio->streamSlot));
}

void playSound(Audio *audio, Wave *wave, int slotId, int volume)
{
  if (slotId >= 0 && slotId < MAX_SOUND_SLOTS)
  {
    platform_lockAudioDevice();
    SoundSlot *slot = &audio->soundSlots[slotId];
    slot->state = PLAYING;
    slot->soundData = wave;
    slot->position = wave->samples;
    slot->remainingSamples = wave->sampleCount;
    slot->volume = CLAMP(volume, 0, MAX_VOLUME);
    platform_unlockAudioDevice();
  }
}

void stopSound(Audio *audio, int slotId)
{
  platform_lockAudioDevice();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if ((slotId == -1 || slotId == i) && slots[i].state == PLAYING)
    {
      slots[i].soundData = NULL;
      slots[i].position = NULL;
      slots[i].state = STOPPED;
      slots[i].remainingSamples = 0;
      slots[i].volume = 0;
    }
  }
  platform_unlockAudioDevice();
}

void stopInstances(Audio *audio, Wave *wave)
{
  platform_lockAudioDevice();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if (slots[i].soundData == wave)
    {
      slots[i].soundData = NULL;
      slots[i].position = NULL;
      slots[i].state = STOPPED;
      slots[i].remainingSamples = 0;
      slots[i].volume = 0;
    }
  }
  platform_unlockAudioDevice();
}

void pauseSound(Audio *audio, int slotId)
{
  platform_lockAudioDevice();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if ((slotId == -1 || slotId == i) && slots[i].state == PLAYING)
      slots[i].state = PAUSED;
  }
  platform_unlockAudioDevice();
}

void resumeSound(Audio *audio, int slotId)
{
  platform_lockAudioDevice();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if ((slotId == -1 || slotId == i) && slots[i].state == PAUSED)
      slots[i].state = PLAYING;
  }
  platform_unlockAudioDevice();
}

SoundState getSoundState(Audio *audio, int slotId)
{
  return slotId >= 0 && slotId < MAX_SOUND_SLOTS ? audio->soundSlots[slotId].state : STOPPED;
}

void playStream(Audio *audio, WaveStream *waveStream, int volume, bool loop)
{
  platform_lockAudioDevice();
  waveStreamSeekStart(waveStream);
  audio->streamSlot.data = waveStream;
  audio->streamSlot.state = PLAYING;
  audio->streamSlot.volume = CLAMP(volume, 0, MAX_VOLUME);
  audio->streamSlot.loop = loop;
  platform_unlockAudioDevice();
}

void stopStream(Audio *audio)
{
  platform_lockAudioDevice();
  audio->streamSlot.state = STOPPED;
  audio->streamSlot.data = NULL;
  audio->streamSlot.volume = 0;
  audio->streamSlot.loop = false;
  platform_unlockAudioDevice();
}

void pauseStream(Audio *audio)
{
  platform_lockAudioDevice();
  if (audio->streamSlot.state == PLAYING)
    audio->streamSlot.state = PAUSED;
  platform_unlockAudioDevice();
}

void resumeStream(Audio *audio)
{
  platform_lockAudioDevice();
  if (audio->streamSlot.state == PAUSED)
    audio->streamSlot.state = PLAYING;
  platform_unlockAudioDevice();
}

void setMasterVolume(Audio *audio, int volume)
{
  audio->masterVolume = CLAMP(volume, 0, MAX_VOLUME);
}

static void __mixSamples(int16_t *destination, const int16_t *source, int numSamples, int numChannels, int volume)
{
  int16_t sourceSample, destSample;
  while (numSamples--)
  {
    sourceSample = (*source++ * volume) / MAX_VOLUME;
    for (int i = 0; i < 3 - numChannels; i++)
    {
      destSample = CLAMP(sourceSample + *destination, S16_MIN, S16_MAX);
      *destination++ = destSample;
    }
  }
}

// We clamp on every mix, which can cause clipping when we're mixing many sounds together.
// Ideally, we'd clamp after all sounds are mixed.
// Haven't had any issues yet, but if we start to get distortion, then this is probably the issue.
void mixSamplesIntoStream(Audio *audio, int16_t *stream, int numSamples)
{
  if (audio->streamSlot.state == PLAYING)
  {
    WaveStream *streamData = audio->streamSlot.data;

    if (streamData->channelCount == 1)
      numSamples /= 2;

    bool finished = readWaveStream(streamData, numSamples, audio->streamSlot.loop);
    __mixSamples(stream, streamData->chunk, streamData->sampleCount, streamData->channelCount, audio->streamSlot.volume);

    if (finished)
      audio->streamSlot.state = STOPPED;
  }

  SoundSlot *slots = audio->soundSlots;

  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if (slots[i].state == PLAYING)
    {
      if (slots[i].remainingSamples > 0)
      {
        int samplesToMix = MIN(slots[i].remainingSamples, numSamples);

        if (slots[i].soundData->channelCount == 1)
          samplesToMix /= 2;

        __mixSamples(stream, slots[i].position, samplesToMix, slots[i].soundData->channelCount, slots[i].volume);
        slots[i].position += samplesToMix;
        slots[i].remainingSamples -= samplesToMix;
      }
      else
      {
        slots[i].soundData = NULL;
        slots[i].position = NULL;
        slots[i].state = STOPPED;
        slots[i].remainingSamples = 0;
        slots[i].volume = 0;
      }
    }
  }
}
