#include <stdbool.h>
#include <string.h>

#include "audio.h"
#include "wav.h"

#define S16_MAX 32767
#define S16_MIN -32768

void initializeAudio(Audio *audio)
{
  memset(audio->sounds, 0, sizeof(audio->sounds));
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(audio->streams, 0, sizeof(audio->streams));

  audio->masterVolume = MAX_VOLUME;
}

void disableAudio(Audio *audio)
{
  for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
    freeWavSound(&audio->sounds[i]);

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
    closeWavStream(&audio->streams[i].data);

  memset(audio->sounds, 0, sizeof(audio->sounds));
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(audio->streams, 0, sizeof(audio->streams));
}

// Assumes that the audio device is already locked.
// Before unloading a sound, we find all sound slots that reference it, and stop them.
static void lockedUnloadSound(SoundData *soundData, SoundSlot *soundSlots)
{
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if (soundSlots[i].soundData == soundData)
    {
      soundSlots[i].soundData = NULL;
      soundSlots[i].position = NULL;
      soundSlots[i].state = STOPPED;
      soundSlots[i].remainingSamples = 0;
      soundSlots[i].volume = 0;
    }
  }

  freeWavSound(soundData);
}

void loadSound(Audio *audio, int soundId, const char *filePath)
{
  if (soundId >= 0 && soundId < MAX_LOADED_SOUNDS)
  {
    audio->lock();

    SoundData *soundData = &audio->sounds[soundId];

    if (soundData->samples != NULL)
      lockedUnloadSound(soundData, audio->soundSlots);

    loadWavSound(soundData, filePath);

    audio->unlock();
  }
}

void unloadSound(Audio *audio, int soundId)
{
  audio->lock();

  SoundData *sounds = audio->sounds;

  for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
  {
    if ((soundId == -1 || soundId == i) && sounds[i].samples != NULL)
      lockedUnloadSound(&sounds[i], audio->soundSlots);
  }

  audio->unlock();
}

void playSound(Audio *audio, int soundId, int slotId, int volume)
{
  if (soundId >= 0 && soundId < MAX_LOADED_SOUNDS && slotId >= 0 && slotId < MAX_SOUND_SLOTS)
  {
    audio->lock();

    SoundData *soundData = &audio->sounds[soundId];

    if (soundData->samples != NULL)
    {
      SoundSlot *slot = &audio->soundSlots[slotId];
      slot->state = PLAYING;
      slot->soundData = soundData;
      slot->position = soundData->samples;
      slot->remainingSamples = soundData->sampleCount;
      slot->volume = CLAMP(volume, 0, MAX_VOLUME);
    }

    audio->unlock();
  }
}

void stopSound(Audio *audio, int slotId)
{
  audio->lock();

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

  audio->unlock();
}

void pauseSound(Audio *audio, int slotId)
{
  audio->lock();

  SoundSlot *slots = audio->soundSlots;

  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if ((slotId == -1 || slotId == i) && slots[i].state == PLAYING)
      slots[i].state = PAUSED;
  }

  audio->unlock();
}

void resumeSound(Audio *audio, int slotId)
{
  audio->lock();

  SoundSlot *slots = audio->soundSlots;

  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if ((slotId == -1 || slotId == i) && slots[i].state == PAUSED)
      slots[i].state = PLAYING;
  }

  audio->unlock();
}

SoundState getSoundState(Audio *audio, int slotId)
{
  return slotId >= 0 && slotId < MAX_SOUND_SLOTS ? audio->soundSlots[slotId].state : STOPPED;
}

void openStream(Audio *audio, int streamId, const char *filePath)
{
  if (streamId >= 0 && streamId < MAX_OPEN_STREAMS)
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamId];

    if (soundStream->data.file != NULL)
      closeWavStream(&soundStream->data);

    openWavStream(&soundStream->data, filePath);

    audio->unlock();
  }
}

void closeStream(Audio *audio, int streamId)
{
  audio->lock();

  SoundStream *streams = audio->streams;

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
  {
    if ((streamId == -1 || streamId == i) && streams[i].data.file != NULL)
    {
      closeWavStream(&streams[i].data);

      streams[i].state = STOPPED;
      streams[i].volume = 0;
      streams[i].loop = false;
    }
  }

  audio->unlock();
}

void playStream(Audio *audio, int streamId, int volume, bool loop)
{
  if (streamId >= 0 && streamId < MAX_OPEN_STREAMS)
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamId];

    if (soundStream->data.file != NULL)
    {
      wavStreamSeekStart(&soundStream->data);

      soundStream->state = PLAYING;
      soundStream->volume = CLAMP(volume, 0, MAX_VOLUME);
      soundStream->loop = loop;
    }

    audio->unlock();
  }
}

void stopStream(Audio *audio, int streamId)
{
  audio->lock();

  SoundStream *streams = audio->streams;

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
  {
    if ((streamId == -1 || streamId == i) && streams[i].state == PLAYING)
      streams[i].state = STOPPED;
  }

  audio->unlock();
}

void pauseStream(Audio *audio, int streamId)
{
  audio->lock();

  SoundStream *streams = audio->streams;

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
  {
    if ((streamId == -1 || streamId == i) && streams[i].state == PLAYING)
      streams[i].state = PAUSED;
  }

  audio->unlock();
}

void resumeStream(Audio *audio, int streamId)
{
  audio->lock();

  SoundStream *streams = audio->streams;

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
  {
    if ((streamId == -1 || streamId == i) && streams[i].state == PAUSED)
      streams[i].state = PLAYING;
  }

  audio->unlock();
}

void setMasterVolume(Audio *audio, int volume)
{
  audio->masterVolume = CLAMP(volume, 0, MAX_VOLUME);
}

static void mixSamples(s16 *destination, const s16 *source, int numSamples, int numChannels, int volume)
{
  s16 sourceSample;
  s16 destSample;

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
// Ideally, we'd mix all of our sounds into a 32 bit buffer, then clamp after all sounds have been mixed.
// Haven't had any issues yet, but if we start to get distortion, then this is probably the issue.
void mixSamplesIntoStream(Audio *audio, s16 *stream, int numSamples)
{
  SoundStream *soundStreams = audio->streams;
  SoundSlot *slots = audio->soundSlots;

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
  {
    if (soundStreams[i].state == PLAYING)
    {
      SoundStreamData *streamData = &soundStreams[i].data;

      if (streamData->channelCount == 1)
        numSamples /= 2;

      bool streamFinished = wavStreamRead(&soundStreams[i].data, numSamples, soundStreams[i].loop);
      mixSamples(stream, streamData->chunk, streamData->sampleCount, soundStreams[i].data.channelCount, soundStreams[i].volume);

      if (streamFinished)
        soundStreams[i].state = STOPPED;
    }
  }

  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if (slots[i].state == PLAYING)
    {
      // If we still have an amount of sound to mix, then do so.
      if (slots[i].remainingSamples > 0)
      {
        int samplesToMix = MIN(slots[i].remainingSamples, numSamples);
        if (slots[i].soundData->channelCount == 1)
          samplesToMix /= 2;

        mixSamples(stream, slots[i].position, samplesToMix, slots[i].soundData->channelCount, slots[i].volume);
        slots[i].position += samplesToMix;
        slots[i].remainingSamples -= samplesToMix;
      }
      // If we've no more samples to mix, then stop the sound.
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
