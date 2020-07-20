#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#define AUDIO_FREQUENCY 44100
#define AUDIO_BITS_PER_SAMPLE 16
#define AUDIO_OUTPUT_CHANNELS 2
#define AUDIO_OUTPUT_SAMPLES 4096
#define AUDIO_CHUNK_SIZE (AUDIO_OUTPUT_SAMPLES * (AUDIO_BITS_PER_SAMPLE * AUDIO_OUTPUT_CHANNELS / 8))
#define MAX_LOADED_SOUNDS 16
#define MAX_SOUND_SLOTS 16
#define MAX_OPEN_STREAMS 2
#define MAX_VOLUME 128

typedef enum {
  STOPPED,
  PLAYING,
  PAUSED
} SoundState;

typedef struct {
  int channelCount;
	int sampleCount;
	s16 *samples;
} SoundData;

typedef struct {
	SoundData *soundData;
	SoundState state;
	int volume;
	int remainingSamples;
  s16 *position;
} SoundSlot;

typedef struct {
  long position;
  long start;
  long end;
  FILE *file;
  int channelCount;
  int sampleCount;
  s16 *chunk;
} SoundStreamData;

typedef struct {
  SoundStreamData data;
  SoundState state;
  int volume;
  bool loop;
} SoundStream;

typedef struct {
	SoundData sounds[MAX_LOADED_SOUNDS];
	SoundSlot soundSlots[MAX_SOUND_SLOTS];
	SoundStream streams[MAX_OPEN_STREAMS];
	int masterVolume;

	void (*lock)();
	void (*unlock)();
} Audio;

void initializeAudio(Audio *audio);
void disableAudio(Audio *audio);
void loadSound(Audio *audio, int soundId, const char *filePath);
void unloadSound(Audio *audio, int soundId);
void playSound(Audio *audio, int soundId, int slotId, int volume);
void stopSound(Audio *audio, int slotId);
void pauseSound(Audio *audio, int slotId);
void resumeSound(Audio *audio, int slotId);
SoundState getSoundState(Audio *audio, int slotId);
void openStream(Audio *audio, int streamId, const char *filePath);
void closeStream(Audio *audio, int streamId);
void playStream(Audio *audio, int streamId, int volume, bool loop);
void stopStream(Audio *audio, int streamId);
void pauseStream(Audio *audio, int streamId);
void resumeStream(Audio *audio, int streamId);
void setMasterVolume(Audio *audio, int volume);
void mixSamplesIntoStream(Audio *audio, s16 *stream, int numSamples);

#endif
