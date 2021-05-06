#ifndef __WAVE_H__
#define __WAVE_H__

#include <stdbool.h>
#include <stdio.h>

// An entire wav loaded into memory.
// These are good for short burst sounds.
typedef struct
{
    int16_t *samples;
    int channelCount;
    int sampleCount;
} Wav;

// A wav that is being streamed.
// The wav file is kept open, and we read chunks at a time.
typedef struct
{
    // Our current read position.
    long position;
    // The starting read position.
    long start;
    // The ending read position.
    long end;
    // The file that we're streaming from.
    FILE *file;
    // The current chunk loaded into memory.
    int16_t *chunk;
    int channelCount;
    int sampleCount;
} WavStream;

Wav *loadWave(const char *filename);

void freeWave(Wav *wave);

WavStream *openWaveStream(const char *filename);

void closeWaveStream(WavStream *waveStream);

bool readWaveStream(WavStream *waveStream, int numSamples, bool loop);

void waveStreamSeekStart(WavStream *waveStream);

#endif
