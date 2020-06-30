#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

// 44,100 Hz / 44,100 samples per second.
#define AUDIO_FREQUENCY 44100

// 2 output channels / stereo.
#define AUDIO_CHANNELS 2

// 16 sounds can be loaded into memory at once. The only memory limit for each individual sound is the hardware.
#define MAX_LOADED_SOUNDS 16

// 16 sounds can be playing concurrently.
#define MAX_SOUND_SLOTS 16

// 2 streams can be open at a single time.
// This is useful for switching back and forth between two music tracks.
#define MAX_OPEN_STREAMS 2

// Max volume that milk uses.
#define MAX_VOLUME 128

typedef enum soundState
{
	STOPPED,
	PLAYING,
	PAUSED
} SoundState;

typedef struct soundData
{
  // The length of the sound in bytes.
	u32 length;

  // Pointer to sample buffer.
	u8 *samples;

  // Number of channels the specific sound. Either 1 (mono) or 2 (stereo).
	u8 channelCount;
} SoundData;

typedef struct soundSlot
{
  // The sound data that the given slot is referencing.
	SoundData *soundData;

  // The state of the sound slot.
	SoundState state;

  // The sound slots volume.
	int volume;

  // The remaining length of the sound slots sound data.
	int remainingLength;

  // The slot's position in the sound data buffer.
	u8 *position;
} SoundSlot;

typedef struct audioStream
{
	// The file handle that is left open in order to read from disk.
	FILE *file;

	// The current read position.
	long position;

	// The starting position of data.
	// This is useful to track in order to loop back to the beginning of sample data.
	long start;

	// The end of data.
	long end;

	// This is filled with every call to stream.
	u8 *chunk;

	// The number of channels in stream.
	u8 channelCount;
} AudioStream;

typedef struct streamSlot
{
	// Pointer to current stream.
	AudioStream *stream;

	// The state of the stream slot.
	SoundState state;

	// Volume of stream.
	int volume;
} StreamSlot;

typedef struct audio
{
  // All loaded sounds. A sound with a non-null buffer is loaded into memory.
	SoundData sounds[MAX_LOADED_SOUNDS];

  // All sound slots. Many slots can reference the same sound data.
	SoundSlot slots[MAX_SOUND_SLOTS];

	// All open streams. A stream with a non-null handle is open.
	AudioStream streams[MAX_OPEN_STREAMS];

	// One playing stream is allowed at a time.
	StreamSlot streamSlot;

  // Master volume of audio will apply to each individual slot's volume.
	int masterVolume;

	void (*lock)();
	void (*unlock)();
} Audio;

// Initializes the audio module.
void initAudio(Audio *audio);

// Frees the audio module and all loaded sounds.
void freeAudio(Audio *audio);

// Load a sound file's data into the given sound index.
void loadSound(Audio *audio, int soundIdx, const char *filename);

// Unload the sound data at the given sound index.
void unloadSound(Audio *audio, int soundIdx);

// Play sound[soundIdx] at the given slot index.
// Slot index 0 will loop the sound.
void playSound(Audio *audio, int slotIdx, int soundIdx, int volume);

// Stop the sound at the given slot index.
// Index -1 will stop all playing slots.
void stopSound(Audio *audio, int slotIdx);

// Pause the sound at the given slot index.
// Index -1 will pause all playing sounds.
void pauseSound(Audio *audio, int slotIdx);

// Resume the sound at the given slot index.
// Index -1 will resume all paused sounds.
void resumeSound(Audio *audio, int slotIdx);

// Get the state of the given slot index.
SoundState getSoundState(Audio *audio, int slotIdx);

// Open a sound file for streaming to the given stream index.
// This is more efficient for music files, since we're only reading chunks at a time as opposed to loading the whole file into memory,
void openStream(Audio *audio, int index, const char *filePath);

// Close out a sound stream.
// This will stop the stream if it is currently playing.
void closeStream(Audio *audio, int index);

// Set the stream at the given index to the currently playing stream.
void playStream(Audio *audio, int index, int volume);

// Stop the stream at the given index if it is playing.
void stopStream(Audio *audio, int index);

// Set the master volume of all sounds.
void setMasterVolume(Audio *audio, int volume);

// Mix all playing sounds into the given stream.
// Milk on it's own does not maintain an audio stream.
// Platform code is responsible for calling this method and specifying the amount of data, in bytes, to mix.
// The destination stream is assumed to be signed, 16 bit samples in the following order LRLRLR.
void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len);

#endif
