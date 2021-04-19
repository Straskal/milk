#ifndef __WAVE_H__
#define __WAVE_H__

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    int16_t *samples;
    int channelCount;
    int sampleCount;
} Wave;

typedef struct
{
    long position;
    long start;
    long end;
    FILE *file;
    int channelCount;
    int sampleCount;
    int16_t *chunk;
} WaveStream;

Wave *loadWave(const char *filename);
void freeWave(Wave *wave);
WaveStream *openWaveStream(const char *filename);
void closeWaveStream(WaveStream *waveStream);
bool readWaveStream(WaveStream *waveStream, int numSamples, bool loop);
void waveStreamSeekStart(WaveStream *waveStream);

#endif
