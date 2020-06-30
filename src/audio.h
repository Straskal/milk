#ifndef __AUDIO_H__
#define __AUDIO_H__

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

// Max volume that milk uses.
#define MAX_VOLUME 128

typedef struct soundData
{
  // The length of the sound in bytes.
	u32 length;

  // Pointer to sample buffer.
	u8 *samples;

  // Number of channels the specific sound. Either 1 (mono) or 2 (stereo).
	u8 channelCount;
} SoundData;

typedef enum soundState
{
	STOPPED,
	PLAYING,
	PAUSED
} SoundState;

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

typedef struct audio
{
  // All loaded sounds. A sound with a non-null buffer is loaded into memory.
	SoundData sounds[MAX_LOADED_SOUNDS];

  // All sound slots. Many slots can reference the same sound data.
	SoundSlot slots[MAX_SOUND_SLOTS];

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

// Set the master volume of all sounds.
void setMasterVolume(Audio *audio, int volume);

// Mix all playing sounds into the given stream.
// Milk on it's own does not maintain an audio stream.
// Platform code is responsible for calling this method and specifying the amount of data, in bytes, to mix.
// The destination stream is assumed to be signed, 16 bit samples in the following order LRLRLR.
void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len);

#endif
