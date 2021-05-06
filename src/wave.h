#ifndef __WAVE_H__
#define __WAVE_H__

#include <stdbool.h>
#include <stdio.h>

// A wav sound is completely loaded into memory.
// These are good for short burst sound effects.
typedef struct
{
    int16_t *samples;
    int sampleCount;
    int channelCount;
} Wave;

// A wav stream is a wav sound, but the file is kept open
// and we continuously read from it as we need more samples.
typedef struct
{
    // The position of our read.
    long position;
    // Where the reader starts.
    long start;
    // Where the reader ends.
    long end;

    // The file that we're streaming.
    FILE *file;
    int channelCount;
    int sampleCount;
    int16_t *chunk;
} WaveStream;

// Loads wav audio.
Wave *loadWave(const char *filename);

// Frees wav audio.
void freeWave(Wave *wave);

// Open wav audio stream.
WaveStream *openWaveStream(const char *filename);

// Close wave audio stream.
void closeWaveStream(WaveStream *waveStream);

// Read the given number of samples from a wav stream.
// If loop is true, the stream will wrap if it ends.
bool readWaveStream(WaveStream *waveStream, int numSamples, bool loop);

// Reset a wav stream.
void waveStreamSeekStart(WaveStream *waveStream);

#endif
