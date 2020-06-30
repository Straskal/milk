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
int loadWavFile(const char* filename, u8 **data, u32 *length, u8 *channelCount);

int openWavStream(const char *filename, AudioStream *stream);
int readFromWavStream(AudioStream *stream, int *length);
void closeWavStream(AudioStream *stream);

#endif
