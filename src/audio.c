#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "audio.h"
#include "wav.h"

// This value is used to clamp values when mixing two 16 bit samples.
#define BIT16_MAX 32767

// Zero-ing out audio's memory resets it to it's default state.
static void zeroOutAudioMem(Audio *audio)
{
  memset(audio->sounds, 0, sizeof(audio->sounds));
  memset(audio->soundSlots, 0, sizeof(audio->soundSlots));
  memset(audio->streams, 0, sizeof(audio->streams));
  memset(&audio->streamSlot, 0, sizeof(audio->streamSlot));
}

void initializeAudio(Audio *audio)
{
	zeroOutAudioMem(audio);
	audio->masterVolume = MAX_VOLUME;
}

void disableAudio(Audio *audio)
{
	for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
    freeWavSound(&audio->sounds[i]);

	for (int i = 0; i < MAX_OPEN_STREAMS; i++)
    closeWavStream(&audio->streams[i]);

  zeroOutAudioMem(audio);
}

// Set the given sound slot to default values.
static void resetSoundSlot(SoundSlot *slot)
{
  memset(slot, 0, sizeof(SoundSlot));
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
// Before unloading the sound, find all slots that reference it, and stop them.
static void lockedUnloadSound(SoundData *soundData, SoundSlot *soundSlots)
{
  for (int i = 0; i < MAX_SOUND_SLOTS; i++)
  {
    if (soundSlots[i].soundData == soundData)
      resetSoundSlot(&soundSlots[i]);
  }

  freeWavSound(soundData);
  memset(soundData, 0, sizeof(SoundData));
}

void loadSound(Audio *audio, int soundIndex, const char *filename)
{
	if (isSoundIndexWithinBounds(soundIndex))
	{
		audio->lock();
		SoundData *sound = &audio->sounds[soundIndex];
		if (sound->samples != NULL) lockedUnloadSound(sound, audio->soundSlots);
		loadWavSound(sound, filename);
		audio->unlock();
	}
}

void unloadSound(Audio *audio, int soundIndex)
{
	if (isSoundIndexWithinBounds(soundIndex))
	{
		audio->lock();
		SoundData *sound = &audio->sounds[soundIndex];
		if (sound->samples != NULL) lockedUnloadSound(sound, audio->soundSlots);
		audio->unlock();
	}
}

void playSound(Audio *audio, int soundIndex, int slotIndex, int volume)
{
	if (isSoundIndexWithinBounds(soundIndex) && isSlotIndexWithinBounds(slotIndex))
	{
		audio->lock();
		SoundData *sampleData = &audio->sounds[soundIndex];

		if (sampleData->samples != NULL)
		{
			SoundSlot *slot = &audio->soundSlots[slotIndex];
			slot->state = PLAYING;
			slot->soundData = sampleData;
			slot->position = sampleData->samples;
			slot->remainingLength = sampleData->length;
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
    if (slot->state == PLAYING) slot->state = PAUSED;
    audio->unlock();
  }
}

void resumeSound(Audio *audio, int slotIndex)
{
  if (isSlotIndexWithinBounds(slotIndex))
  {
    audio->lock();
    SoundSlot *slot = &audio->soundSlots[slotIndex];
    if (slot->state == PAUSED) slot->state = PLAYING;
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
  if (streamSlot->stream == audioStream) resetStreamSlot(streamSlot);
  closeWavStream(audioStream);
  memset(audioStream, 0, sizeof(SoundStream));
}

void openStream(Audio *audio, int streamIndex, const char *filePath)
{
	if (isStreamIndexWithinBounds(streamIndex))
	{
		audio->lock();
		SoundStream *audioStream = &audio->streams[streamIndex];
		if (audioStream->file != NULL) lockedCloseStream(audioStream, &audio->streamSlot);
		openWavStream(audioStream, filePath);
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

void playStream(Audio *audio, int streamIndex, int volume)
{
	if (isStreamIndexWithinBounds(streamIndex))
	{
		audio->lock();
		SoundStream *audioStream = &audio->streams[streamIndex];

		if (audioStream->file != NULL)
    {
		  StreamSlot *streamSlot = &audio->streamSlot;
      streamSlot->state = PLAYING;
      streamSlot->stream = audioStream;
      streamSlot->volume = CLAMP(volume, 0, MAX_VOLUME);
    }
		audio->unlock();
	}
}

void stopStream(Audio *audio, int streamIndex)
{
	if (isStreamIndexWithinBounds(streamIndex))
	{
		audio->lock();
    resetStreamSlot(&audio->streamSlot);
		audio->unlock();
	}
}

void setMasterVolume(Audio *audio, int volume)
{
	audio->masterVolume = CLAMP(volume, 0, MAX_VOLUME);
}

// Mixes a stereo sound into the given destination buffer.
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
		mixedSample = CLAMP(sourceSample + destSample, -BIT16_MAX - 1, BIT16_MAX);
		destination[0] = mixedSample & 0xff;
		destination[1] = (mixedSample >> 8) & 0xff;
		source += 2;
		destination += 2;
	}
}

// Interleaves a mono sound into the given destination buffer.
// This involves straight up playing each mono sample in both left and right channels, effectively doubling everything.
// We interleave mono sounds at mix time as opposed to converting the mono to stereo, so we don't have to double our memory usage.
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
		mixedSample = CLAMP(sourceSample + destSample, -BIT16_MAX - 1, BIT16_MAX);
		destination[0] = mixedSample & 0xff;
		destination[1] = (mixedSample >> 8) & 0xff;
		destSample = destination[3] << 8 | destination[2];
		mixedSample = CLAMP(sourceSample + destSample, -BIT16_MAX - 1, BIT16_MAX);
		destination[2] = mixedSample & 0xff;
		destination[3] = (mixedSample >> 8) & 0xff;
		source += 2;
		destination += 4;
	}
}

void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len)
{
	memset(stream, 0, len);

	// If we're streaming audio, then read a new chunk and mix it into the audio device stream.
	StreamSlot *streamSlot = &audio->streamSlot;

	if (streamSlot->state == PLAYING)
	{
		if (readFromWavStream(streamSlot->stream)) resetWavStream(streamSlot->stream);
		mixStereoSamples(stream, streamSlot->stream->chunk, streamSlot->stream->chunkLength, streamSlot->volume);
	}

	// Mix all playing sound slots.
	for (int i = 0; i < MAX_SOUND_SLOTS; i++)
	{
		SoundSlot *slot = &audio->soundSlots[i];

		if (slot->state == PLAYING)
		{
		  // If we still have an amount of sound to mix, then do so.
		  // Track the amount that we wrote, and move the sound position forward.
      if (slot->remainingLength > 0)
      {
        int bytesToWrite = MIN(slot->remainingLength, (int)len);

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
      else
      {
        slot->soundData = NULL;
        slot->state = STOPPED;
      }
		}
	}
}
