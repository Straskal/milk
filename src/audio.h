#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

// 44,100 Hz / 44,100 samples per second.
#define AUDIO_FREQUENCY 44100

// Samples are expected to be signed 16 bit integers.
#define AUDIO_BITS_PER_SAMPLE 16

// Output stereo sounds / 2 channels.
#define AUDIO_OUTPUT_CHANNELS 2

// The number of samples that are output when mixing.
#define AUDIO_OUTPUT_SAMPLES 4096

// The size (in bytes) of each chunk to mix.
// Example:
//  Each sample being 16 bits. In other words, each sample is 2 bytes.
//  If we're outputting 2 channels, then we're outputting 2x the samples. 4 bytes.
//  We want to output 4096 samples per mix. 4096 samples at 4 bytes each = 16384 bytes per mix.
//  This would be the size to use if we ever want to maintain our own internal stream.
//  For now, this will only drive the size of our sound stream chunks.
#define AUDIO_CHUNK_SIZE (AUDIO_OUTPUT_SAMPLES * (AUDIO_BITS_PER_SAMPLE * AUDIO_OUTPUT_CHANNELS / 8))

// 16 sounds can be loaded into memory at once. The only memory limit for each individual sound is the hardware.
#define MAX_LOADED_SOUNDS 16

// 16 sounds can be playing concurrently.
#define MAX_SOUND_SLOTS 16

// 2 streams can be open at a time to enable things like switching between to music tracks.
#define MAX_OPEN_STREAMS 2

#define MAX_VOLUME 128

typedef enum
{
	PLAYING,
	STOPPED
} SoundState;

// Sound data represents an entire sound loaded into memory.
// This is ideal for short-burst sounds, since they are typically small.
typedef struct
{
	int sampleCount;
	s16 *samples;
	u8 channelCount;
} SoundData;

// Sound slots are what we insert sound data in to in order to play them.
// Many slots can reference a single sound data, and do their own book-keeping to track the sounds current position, volume, etc..
typedef struct
{
	SoundData *soundData;
	SoundState state;
	int volume;
	int remainingSamples;
  s16 *position;
} SoundSlot;

// Sound streams are ideal for sounds with a larger memory footprint (music).
// Instead of loading an entire music file into memory, we load chunks at a time from the disk.
typedef struct
{
  long position;
	long start;
	long end;
	FILE *file;
  SoundState state;
  int volume;
	int chunkSampleCount;
  s16 *chunk;
  bool loop;
	u8 channelCount;
} SoundStream;

typedef struct
{
	SoundData sounds[MAX_LOADED_SOUNDS];
	SoundSlot soundSlots[MAX_SOUND_SLOTS];
	SoundStream streams[MAX_OPEN_STREAMS];
	int masterVolume;

	// The audio device is platform specific, but some audio device libraries run the device stream on a separate thread.
	// Both lock and unlock functions are necessary to call whenever modifying our sound data.
	// This tells the audio device "Hey, don't request us to mix anything [on another thread] into your audio stream at the moment."
	void (*lock)();
	void (*unlock)();
} Audio;

// Initializes the audio module.
//
void initializeAudio(Audio *audio);

// resets the audio module and frees all loaded sounds.
//
void disableAudio(Audio *audio);

// Load a sound file's data into the given sound index.
//
void loadSound(Audio *audio, int soundIndex, const char *filePath);

// Unload the sound data at the given sound index.
//
void unloadSound(Audio *audio, int soundIndex);

// Play sound[soundIdx] at the given slot index.
// Slot index 0 will loop the sound.
//
void playSound(Audio *audio, int soundIndex, int slotIndex, int volume);

// Stop the sound at the given slot index.
// slot index -1 will stop all playing sounds.
//
void stopSound(Audio *audio, int slotIndex);

// Resume the sound at the given slot index.
//
void resumeSound(Audio *audio, int slotIndex);

// Get the state of the given slot index.
//
SoundState getSoundState(Audio *audio, int slotIndex);

// Open a sound file for streaming to the given stream index.
// This is more efficient for music files, since we're only reading chunks at a time as opposed to loading the whole file into memory.
//
void openStream(Audio *audio, int streamIndex, const char *filePath);

// Close out a sound stream.
// This will stop the stream if it is currently playing.
//
void closeStream(Audio *audio, int streamIndex);

// Set the stream at the given index to the currently playing stream.
//
void playStream(Audio *audio, int streamIndex, int volume, bool loop);

// Stop the stream if it is playing.
//
void stopStream(Audio *audio, int streamIndex);

// Resume the stream index if it is paused.
//
void resumeStream(Audio *audio, int streamIndex);

// Set the master volume of all sounds.
//
void setMasterVolume(Audio *audio, int volume);

// Mix all playing sounds into the given stream.
// Milk on it's own does not maintain an audio stream.
// Platform code is responsible for calling this method and specifying the amount of data, in samples, to mix.
// The destination stream is assumed to be signed, 16 bit samples in the following order [LRLRLR].
//
void mixSamplesIntoStream(Audio *audio, s16 *stream, int numSamples);

#endif
