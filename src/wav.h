#ifndef __WAV_H__
#define __WAV_H__

#include <stdio.h>

#include "audio.h"
#include "common.h"

#define CHUNK_SIZE 16384

// There are limits to what kind of wave files we can load.
// - Mono and stereo PCM
// - Signed 16 bit samples
// - 44,100 sample rate.

int loadWavSound(SoundData *soundData, const char *filename);
void freeWavSound(SoundData *soundData);
int openWavStream(SoundStream *stream, const char *filename);
int readFromWavStream(SoundStream *stream);
void closeWavStream(SoundStream *stream);

#endif
