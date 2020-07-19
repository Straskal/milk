#ifndef __WAV_H__
#define __WAV_H__

#include <stdbool.h>
#include <stdio.h>

#include "audio.h"
#include "common.h"

bool loadWavSound(SoundData *soundData, const char *filename);
void freeWavSound(SoundData *soundData);
bool openWavStream(SoundStreamData *streamData, const char *filename);
void closeWavStream(SoundStreamData *streamData);
bool wavStreamRead(SoundStreamData *streamData, int numSamples, bool loop);
void wavStreamSeekStart(SoundStreamData *streamData);

#endif
