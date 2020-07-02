#ifndef __WAV_H__
#define __WAV_H__

#include <stdbool.h>
#include <stdio.h>

#include "audio.h"
#include "common.h"

// Loads a wav sound data into memory or returns false if an error occurs.
//
bool loadWavSound(SoundData *soundData, const char *filename);

// Frees wav sound data.
//
void freeWavSound(SoundData *soundData);

// Opens a new wav data stream or returns false if an error occurs.
//
bool openWavStream(SoundStream *stream, const char *filename);

// Closes a wav stream.
//
void closeWavStream(SoundStream *stream);

// Reads one chunk from the stream and returns true if the stream has reached its end.
// Will always return false if loop is true.
//
bool readFromWavStream(SoundStream *stream, int numSamples, bool loop);

// Resets the wav stream to the beginning of data. This is ideal for looping.
//
void resetWavStream(SoundStream *stream);

#endif
