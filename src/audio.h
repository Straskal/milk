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
	u8  *samples;

  // Number of channels the specific sound.
	u8  channelCount;
} SoundData;

typedef enum soundState
{
	STOPPED,
	PLAYING,
	PAUSED
} SoundState;

typedef struct soundSlot
{
	SoundData   *soundData;
	SoundState  state;
	int         volume;
	int         remainingLength;
	u8          *position;
} SoundSlot;

typedef struct audio
{
	SoundData sounds[MAX_LOADED_SOUNDS];
	SoundSlot slots[MAX_SOUND_SLOTS];
	int       masterVolume;

	void (*lock)();
	void (*unlock)();
} Audio;

/**
 * Initialize audio.
 * @param audio
 */
void initAudio(Audio *audio);

/**
 * Free audio and all loaded sounds.
 * @param audio
 */
void freeAudio(Audio *audio);

/**
 * Load a sound file's data into the given sound index.
 * @param audio
 * @param soundIdx
 * @param filename
 */
void loadSound(Audio *audio, int soundIdx, const char *filename);

/**
 * Unload the sound data at the given sound index.
 * @param audio
 * @param soundIdx
 */
void unloadSound(Audio *audio, int soundIdx);

/**
 * Play sound[soundIdx] at the given slot index.
 * @param audio
 * @param slotIdx
 * @param soundIdx
 * @param volume
 */
void playSound(Audio *audio, int slotIdx, int soundIdx, int volume);

/**
 * Stop the sound at the given slot index.
 * @param audio
 * @param slotIdx
 */
void stopSound(Audio *audio, int slotIdx);

/**
 * Pause the sound at the given slot index.
 * @param audio
 * @param slotIdx
 */
void pauseSound(Audio *audio, int slotIdx);

/**
 * Resume the sound at the given slot index.
 * @param audio
 * @param slotIdx
 */
void resumeSound(Audio *audio, int slotIdx);

/**
 * Get the state of the given slot index.
 * @param audio
 * @param slotIdx
 * @return
 */
SoundState getSoundState(Audio *audio, int slotIdx);

/**
 * Set the master volume of all sounds.
 * @param audio
 * @param volume
 */
void setMasterVolume(Audio *audio, int volume);

/**
 * Mix all playing sounds into the given stream.
 * @param audio
 * @param stream
 * @param len
 */
void mixSamplesIntoStream(Audio *audio, u8 *stream, size_t len);

#endif
