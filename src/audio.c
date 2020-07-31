#include <stdbool.h>
#include <string.h>

#include "audio.h"
#include "common.h"
#include "wave.h"

#define S16_MAX 32767
#define S16_MIN -32768

void initializeAudio(Audio *audio) {
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(&audio->streamSlot, 0, sizeof(audio->streamSlot));
  audio->masterVolume = MAX_VOLUME;
}

void disableAudio(Audio *audio) {
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(&audio->streamSlot, 0, sizeof(audio->streamSlot));
}

void playSound(Audio *audio, Wave *wave, int slotId, int volume) {
  if (slotId >= 0 && slotId < MAX_SOUND_SLOTS) {
    audio->lock();
    SoundSlot *slot = &audio->soundSlots[slotId];
    slot->state = PLAYING;
    slot->soundData = wave;
    slot->position = wave->samples;
    slot->remainingSamples = wave->sampleCount;
    slot->volume = CLAMP(volume, 0, MAX_VOLUME);
    audio->unlock();
  }
}

void stopSound(Audio *audio, int slotId) {
  audio->lock();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++) {
    if ((slotId == -1 || slotId == i) && slots[i].state == PLAYING) {
      slots[i].soundData = NULL;
      slots[i].position = NULL;
      slots[i].state = STOPPED;
      slots[i].remainingSamples = 0;
      slots[i].volume = 0;
    }
  }
  audio->unlock();
}

void stopInstances(Audio *audio, Wave *wave) {
  audio->lock();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++) {
    if (slots[i].soundData == wave) {
      slots[i].soundData = NULL;
      slots[i].position = NULL;
      slots[i].state = STOPPED;
      slots[i].remainingSamples = 0;
      slots[i].volume = 0;
    }
  }
  audio->unlock();
}

void pauseSound(Audio *audio, int slotId) {
  audio->lock();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++) {
    if ((slotId == -1 || slotId == i) && slots[i].state == PLAYING)
      slots[i].state = PAUSED;
  }
  audio->unlock();
}

void resumeSound(Audio *audio, int slotId) {
  audio->lock();
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++) {
    if ((slotId == -1 || slotId == i) && slots[i].state == PAUSED)
      slots[i].state = PLAYING;
  }
  audio->unlock();
}

SoundState getSoundState(Audio *audio, int slotId) {
  return slotId >= 0 && slotId < MAX_SOUND_SLOTS ? audio->soundSlots[slotId].state : STOPPED;
}

void playStream(Audio *audio, WaveStream *waveStream, int volume, bool loop) {
  audio->lock();
  waveStreamSeekStart(waveStream);
  audio->streamSlot.data = waveStream;
  audio->streamSlot.state = PLAYING;
  audio->streamSlot.volume = CLAMP(volume, 0, MAX_VOLUME);
  audio->streamSlot.loop = loop;
  audio->unlock();
}

void stopStream(Audio *audio) {
  audio->lock();
  if (audio->streamSlot.state == PLAYING)
    audio->streamSlot.state = STOPPED;
  audio->unlock();
}

void pauseStream(Audio *audio) {
  audio->lock();
  if (audio->streamSlot.state == PLAYING)
    audio->streamSlot.state = PAUSED;
  audio->unlock();
}

void resumeStream(Audio *audio) {
  audio->lock();
  if (audio->streamSlot.state == PAUSED)
    audio->streamSlot.state = PLAYING;
  audio->unlock();
}

void setMasterVolume(Audio *audio, int volume) {
  audio->masterVolume = CLAMP(volume, 0, MAX_VOLUME);
}

static void mixSamples(int16_t *destination, const int16_t *source, int numSamples, int numChannels, int volume) {
  int16_t sourceSample, destSample;
  while (numSamples--) {
    sourceSample = (*source++ * volume) / MAX_VOLUME;
    for (int i = 0; i < 3 - numChannels; i++) {
      destSample = CLAMP(sourceSample + *destination, S16_MIN, S16_MAX);
      *destination++ = destSample;
    }
  }
}

// We clamp on every mix, which can cause clipping when we're mixing many sounds together.
// Ideally, we'd clamp after all sounds are mixed.
// Haven't had any issues yet, but if we start to get distortion, then this is probably the issue.
void mixSamplesIntoStream(Audio *audio, int16_t *stream, int numSamples) {
  if (audio->streamSlot.state == PLAYING) {
    WaveStream *streamData = audio->streamSlot.data;
    if (streamData->channelCount == 1) numSamples /= 2;
    bool finished = readWaveStream(streamData, numSamples, audio->streamSlot.loop);
    mixSamples(stream, streamData->chunk, streamData->sampleCount, streamData->channelCount, audio->streamSlot.volume);
    if (finished) audio->streamSlot.state = STOPPED;
  }
  SoundSlot *slots = audio->soundSlots;
  for (int i = 0; i < MAX_SOUND_SLOTS; i++) {
    if (slots[i].state == PLAYING) {
      if (slots[i].remainingSamples > 0) {
        int samplesToMix = MIN(slots[i].remainingSamples, numSamples);
        if (slots[i].soundData->channelCount == 1) samplesToMix /= 2;
        mixSamples(stream, slots[i].position, samplesToMix, slots[i].soundData->channelCount, slots[i].volume);
        slots[i].position += samplesToMix;
        slots[i].remainingSamples -= samplesToMix;
      } else {
        slots[i].soundData = NULL;
        slots[i].position = NULL;
        slots[i].state = STOPPED;
        slots[i].remainingSamples = 0;
        slots[i].volume = 0;
      }
    }
  }
}
