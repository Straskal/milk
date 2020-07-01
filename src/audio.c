#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "audio.h"
#include "wav.h"

// This value is used to clamp values when mixing two 16 bit samples.
#define S16_MAX 32767

// Zero-ing out audio's memory resets it to it's default state.
static void zeroOutAudioMemory(Audio *audio)
{
  memset(audio->sounds, 0, sizeof(audio->sounds));
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(audio->streams, 0, sizeof(audio->streams));
  memset(&audio->streamSlot, 0, sizeof(audio->streamSlot));
}

void initializeAudio(Audio *audio)
{
  zeroOutAudioMemory(audio);
  audio->masterVolume = MAX_VOLUME;
}

void disableAudio(Audio *audio)
{
  for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
    freeWavSound(&audio->sounds[i]);

  for (int i = 0; i < MAX_OPEN_STREAMS; i++)
    closeWavStream(&audio->streams[i]);

  zeroOutAudioMemory(audio);
}

static void resetSoundSlot(SoundSlot *slot)
{
  slot->soundData = NULL;
  slot->position = NULL;
  slot->state = STOPPED;
  slot->remainingSamples = 0;
  slot->volume = 0;
}

static bool isSoundIndexWithinBounds(int index)
{
  return index >= 0 && index < MAX_LOADED_SOUNDS;
}

static bool isSlotIndexWithinBounds(int index)
{
  return index >= 0 && index < MAX_SOUND_SLOTS;
}

// Assumes that the audio device is already locked.
static void lockedUnloadSound(SoundData *soundData, SoundSlot *soundSlots)
{
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    // Before unloading the sound, find all slots that reference it, and stop them.
    if (soundSlots[i].soundData == soundData)
      resetSoundSlot(&soundSlots[i]);
  }

  freeWavSound(soundData);

  soundData->samples = NULL;
  soundData->sampleCount = 0;
  soundData->channelCount = 0;
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
  if (isSlotIndexWithinBounds(slotIndex))
  {
    audio->lock();

    SoundSlot *slot = &audio->soundSlots[slotIndex];
    resetSoundSlot(slot);

    audio->unlock();
  }
}

void pauseSound(Audio *audio, int slotIndex)
{
  if (isSlotIndexWithinBounds(slotIndex))
  {
    audio->lock();

    SoundSlot *slot = &audio->soundSlots[slotIndex];

    if (slot->state == PLAYING)
      slot->state = PAUSED;

    audio->unlock();
  }
}

void resumeSound(Audio *audio, int slotIndex)
{
  if (isSlotIndexWithinBounds(slotIndex))
  {
    audio->lock();

    SoundSlot *slot = &audio->soundSlots[slotIndex];

    if (slot->state == PAUSED)
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

static void resetStreamSlot(StreamSlot *streamSlot)
{
  memset(streamSlot, 0, sizeof(StreamSlot));
}

static void lockedCloseStream(SoundStream *audioStream, StreamSlot *streamSlot)
{
  if (streamSlot->stream == audioStream)
    resetStreamSlot(streamSlot);

  closeWavStream(audioStream);
  memset(audioStream, 0, sizeof(SoundStream));
}

void openStream(Audio *audio, int streamIndex, const char *filePath)
{
  if (isStreamIndexWithinBounds(streamIndex))
  {
    audio->lock();

    SoundStream *soundStream = &audio->streams[streamIndex];

    if (soundStream->file != NULL)
      lockedCloseStream(soundStream, &audio->streamSlot);

    openWavStream(soundStream, filePath);

    audio->unlock();
  }
}

void closeStream(Audio *audio, int streamIndex)
{
  if (isStreamIndexWithinBounds(streamIndex))
  {
    audio->lock();

    lockedCloseStream(&audio->streams[streamIndex], &audio->streamSlot);

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
      resetWavStream(soundStream);

      StreamSlot *slot = &audio->streamSlot;
      slot->state = PLAYING;
      slot->stream = soundStream;
      slot->volume = CLAMP(volume, 0, MAX_VOLUME);
      slot->loop = loop;
    }

    audio->unlock();
  }
}

void stopStream(Audio *audio)
{
  audio->lock();

  resetWavStream(audio->streamSlot.stream);
  resetStreamSlot(&audio->streamSlot);

  audio->unlock();
}

void pauseStream(Audio *audio)
{
  audio->lock();

  if (audio->streamSlot.state == PLAYING)
    audio->streamSlot.state = PAUSED;

  audio->unlock();
}

void resumeStream(Audio *audio)
{
  audio->lock();

  if (audio->streamSlot.state == PAUSED)
    audio->streamSlot.state = PLAYING;

  audio->unlock();
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
    destSample = CLAMP(sourceSample + *destination, -S16_MAX - 1, S16_MAX);
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
  numSamples /= sizeof(s16);

  while (numSamples--)
  {
    sourceSample = (*source++ * volume) / MAX_VOLUME;
    destSample = CLAMP(sourceSample + *destination, -S16_MAX - 1, S16_MAX);
    *destination++ = destSample;
    destSample = CLAMP(sourceSample + *destination, -S16_MAX - 1, S16_MAX);
    *destination++ = destSample;
  }
}

// We clamp on every mix, which can cause clipping when we're mixing many sounds together.
// Ideally, we'd mix all of our sounds into a 32 bit buffer, then clamp after all sounds have been mixed.
// Haven't had any issues yet, but if we start to get distortion, then this is probably the issue.
void mixSamplesIntoStream(Audio *audio, s16 *stream, int numSamples)
{
  // If we're streaming audio, then read a new chunk and mix it into the audio device stream.
  StreamSlot *streamSlot = &audio->streamSlot;

  if (streamSlot->state == PLAYING)
  {
    bool streamFinished = readFromWavStream(streamSlot->stream, numSamples, streamSlot->loop);

    // TODO: Assuming that the music is stereo.
    mixStereoSamples(stream, streamSlot->stream->chunk, streamSlot->stream->chunkSampleCount, streamSlot->volume);

    if (streamFinished)
      resetStreamSlot(streamSlot);
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

        if (slot->soundData->channelCount == 1)
        {
          mixInterleavedMonoSamples(stream, slot->position, samplesToMix, slot->volume);

          int halved = (int) (samplesToMix / sizeof(s16));
          slot->position += halved;
          slot->remainingSamples -= halved;
        }
        else
        {
          mixStereoSamples(stream, slot->position, samplesToMix, slot->volume);

          slot->position += samplesToMix;
          slot->remainingSamples -= samplesToMix;
        }
      }
      // If we've no more samples to mix, then stop the sound.
      else resetSoundSlot(slot);
    }
  }
}
