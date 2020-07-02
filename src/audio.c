#include <stdbool.h>
#include <string.h>

#include "audio.h"
#include "wav.h"

#define S16_MAX 32767
#define S16_MIN -32768

// Right now, we can just rely on memset to initialize our memory to default state.
static void initializeMemory(Audio *audio)
{
  memset(audio->sounds, 0, sizeof(audio->sounds));
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(audio->streams, 0, sizeof(audio->streams));
}

void initializeAudio(Audio *audio)
{
  initializeMemory(audio);

  audio->masterVolume = MAX_VOLUME;
}

void disableAudio(Audio *audio)
{
  for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
    freeWavSound(&audio->sounds[i]);

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
    closeWavStream(&audio->streams[i]);

  initializeMemory(audio);
}

// Reset sound slot to default state.
static void resetSoundSlot(SoundSlot *slot)
{
  slot->soundData = NULL;
  slot->position = NULL;
  slot->state = STOPPED;
  slot->remainingSamples = 0;
  slot->volume = 0;
}

// Assumes that the audio device is already locked.
// Before unloading a sound, we find all sound slots that reference it, and stop them.
static void lockedUnloadSound(SoundData *soundData, SoundSlot *soundSlots)
{
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    SoundSlot *slot = &soundSlots[i];

    if (slot->soundData == soundData)
      resetSoundSlot(slot);
  }

  freeWavSound(soundData);
}

static bool isSoundIndexWithinBounds(int index)
{
  return index >= 0 && index < MAX_LOADED_SOUNDS;
}

static bool isSlotIndexWithinBounds(int index)
{
  return index >= 0 && index < MAX_SOUND_SLOTS;
}

void loadSound(Audio *audio, int soundIndex, const char *filePath)
{
  if (isSoundIndexWithinBounds(soundIndex))
  {
    audio->lock();

    SoundData *soundData = &audio->sounds[soundIndex];

    if (soundData->samples != NULL)
      lockedUnloadSound(soundData, audio->soundSlots);

    loadWavSound(soundData, filePath);

    audio->unlock();
  }
}

void unloadSound(Audio *audio, int soundIndex)
{
  if (isSoundIndexWithinBounds(soundIndex))
  {
    audio->lock();

    SoundData *soundData = &audio->sounds[soundIndex];

    if (soundData->samples != NULL)
      lockedUnloadSound(soundData, audio->soundSlots);

    audio->unlock();
  }
}

void playSound(Audio *audio, int soundIndex, int slotIndex, int volume)
{
  if (isSoundIndexWithinBounds(soundIndex) && isSlotIndexWithinBounds(slotIndex))
  {
    audio->lock();

    SoundData *soundData = &audio->sounds[soundIndex];

    if (soundData->samples != NULL)
    {
      SoundSlot *slot = &audio->soundSlots[slotIndex];
      slot->state = PLAYING;
      slot->soundData = soundData;
      slot->position = soundData->samples;
      slot->remainingSamples = soundData->sampleCount;
      slot->volume = CLAMP(volume, 0, MAX_VOLUME);
    }

    audio->unlock();
  }
}

void stopSound(Audio *audio, int slotIndex)
{
  if (slotIndex == -1)
  {
    audio->lock();

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
    {
      SoundSlot *slot = &audio->soundSlots[i];

      if (slot->state == PLAYING)
        slot->state = STOPPED;
    }

    audio->unlock();
  }
  else if (isSlotIndexWithinBounds(slotIndex))
  {
    audio->lock();

    SoundSlot *slot = &audio->soundSlots[slotIndex];

    if (slot->state == PLAYING)
      slot->state = STOPPED;

    audio->unlock();
  }
}

void resumeSound(Audio *audio, int slotIndex)
{
  if (slotIndex == -1)
  {
    audio->lock();

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
    {
      SoundSlot *slot = &audio->soundSlots[i];

      if (slot->soundData != NULL && slot->state == STOPPED)
        slot->state = PLAYING;
    }

    audio->unlock();
  }
  else if (isSlotIndexWithinBounds(slotIndex))
  {
    audio->lock();

    SoundSlot *slot = &audio->soundSlots[slotIndex];

    if (slot->soundData != NULL && slot->state == STOPPED)
      slot->state = PLAYING;

    audio->unlock();
  }
}

SoundState getSoundState(Audio *audio, int slotIndex)
{
  return isSlotIndexWithinBounds(slotIndex) ? audio->soundSlots[slotIndex].state : STOPPED;
}

static bool isStreamIndexWithinBounds(int index)
{
  return index >= 0 && index < MAX_OPEN_STREAMS;
}

void openStream(Audio *audio, int streamIndex, const char *filePath)
{
  if (isStreamIndexWithinBounds(streamIndex))
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamIndex];

    if (soundStream->file != NULL)
      closeWavStream(soundStream);

    openWavStream(soundStream, filePath);

    audio->unlock();
  }
}

void closeStream(Audio *audio, int streamIndex)
{
  if (isStreamIndexWithinBounds(streamIndex))
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamIndex];

    if (soundStream->file != NULL)
      closeWavStream(soundStream);

    audio->unlock();
  }
}

void playStream(Audio *audio, int streamIndex, int volume, bool loop)
{
  if (isStreamIndexWithinBounds(streamIndex))
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamIndex];

    if (soundStream->file != NULL)
    {
      moveWavStreamToStart(soundStream);

      soundStream->state = PLAYING;
      soundStream->volume = CLAMP(volume, 0, MAX_VOLUME);
      soundStream->loop = loop;
    }

    audio->unlock();
  }
}

void stopStream(Audio *audio, int streamIndex)
{
  if (streamIndex == -1)
  {
    audio->lock();

    for (int i = 0; i < MAX_OPEN_STREAMS; i++)
    {
      SoundStream *soundStream = &audio->streams[i];

      if (soundStream->state == PLAYING)
        soundStream->state = STOPPED;
    }

    audio->unlock();
  }
  else if (isStreamIndexWithinBounds(streamIndex))
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamIndex];

    if (soundStream->state == PLAYING)
      soundStream->state = STOPPED;

    audio->unlock();
  }
}

void resumeStream(Audio *audio, int streamIndex)
{
  if (streamIndex == -1)
  {
    audio->lock();

    for (int i = 0; i < MAX_OPEN_STREAMS; i++)
    {
      SoundStream *soundStream = &audio->streams[i];

      if (soundStream->file != NULL && soundStream->state == STOPPED)
        soundStream->state = PLAYING;
    }

    audio->unlock();
  }
  else if (isStreamIndexWithinBounds(streamIndex))
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamIndex];

    if (soundStream->file != NULL &&soundStream->state == STOPPED)
      soundStream->state = PLAYING;

    audio->unlock();
  }
}

void setMasterVolume(Audio *audio, int volume)
{
  audio->masterVolume = CLAMP(volume, 0, MAX_VOLUME);
}

// Mixes a stereo sound into the given destination buffer.
static void mixStereoSamples(s16 *destination, const s16 *source, int numSamples, int volume)
{
  s16 sourceSample;
  s16 destSample;

  while (numSamples--)
  {
    sourceSample = (*source++ * volume) / MAX_VOLUME;
    destSample = CLAMP(sourceSample + *destination, S16_MIN, S16_MAX);
    *destination++ = destSample;
  }
}

// Interleaves a mono sound into the given destination buffer.
// This involves straight up playing each mono sample in both left and right channels, effectively doubling everything.
// We interleave mono sounds at mix time as opposed to converting the mono to stereo, so we don't have to double our memory usage.
static void mixInterleavedMonoSamples(s16 *destination, const s16 *source, int numSamples, int volume)
{
  s16 sourceSample;
  s16 destSample;

  while (numSamples--)
  {
    sourceSample = (*source++ * volume) / MAX_VOLUME;
    destSample = CLAMP(sourceSample + *destination, S16_MIN, S16_MAX);
    *destination++ = destSample;
    destSample = CLAMP(sourceSample + *destination, S16_MIN, S16_MAX);
    *destination++ = destSample;
  }
}

// We clamp on every mix, which can cause clipping when we're mixing many sounds together.
// Ideally, we'd mix all of our sounds into a 32 bit buffer, then clamp after all sounds have been mixed.
// Haven't had any issues yet, but if we start to get distortion, then this is probably the issue.
void mixSamplesIntoStream(Audio *audio, s16 *stream, int numSamples)
{
  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
  {
    SoundStream *soundStream = &audio->streams[i];

    if (soundStream->state == PLAYING)
    {
      bool streamFinished = false;

      switch (soundStream->channelCount)
      {
        case 1:
          streamFinished = readFromWavStream(soundStream, numSamples / 2, soundStream->loop);
          mixInterleavedMonoSamples(stream, soundStream->chunk, soundStream->chunkSampleCount, soundStream->volume);
          break;
        case 2:
          streamFinished = readFromWavStream(soundStream, numSamples, soundStream->loop);
          mixStereoSamples(stream, soundStream->chunk, soundStream->chunkSampleCount, soundStream->volume);
          break;
        default:
          break; // This should never happen.
      }

      if (streamFinished)
        soundStream->state = STOPPED;
    }
  }

  // Mix all playing sound slots.
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    SoundSlot *slot = &audio->soundSlots[i];

    if (slot->state == PLAYING)
    {
      // If we still have an amount of sound to mix, then do so.
      if (slot->remainingSamples > 0)
      {
        int samplesToMix = MIN(slot->remainingSamples, numSamples);

        switch (slot->soundData->channelCount)
        {
          case 1:
            // Interleaving a mono signal is just streaming each sample into both the left and right stereo channels.
            // So when we mix, we only mix half of the requested amount of samples.
            samplesToMix /= 2;
            mixInterleavedMonoSamples(stream, slot->position, samplesToMix, slot->volume);
            break;
          case 2:
            mixStereoSamples(stream, slot->position, samplesToMix, slot->volume);
            break;
          default:
            break; // This should never happen.
        }

        slot->position += samplesToMix;
        slot->remainingSamples -= samplesToMix;
      }
      // If we've no more samples to mix, then stop the sound.
      else resetSoundSlot(slot);
    }
  }
}
