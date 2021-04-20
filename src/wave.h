#ifndef __WAVE_H__
#define __WAVE_H__

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    int16_t *samples;
    int channel_count;
    int sample_count;
} Wave;

typedef struct
{
    long position;
    long start;
    long end;
    FILE *file;
    int channel_count;
    int sample_count;
    int16_t *chunk;
} WaveStream;

Wave *wav_load(const char *filename);
void wav_free(Wave *wave);
WaveStream *wav_open_stream(const char *filename);
void wav_close_stream(WaveStream *wav_stream);
bool wav_read_stream(WaveStream *wav_stream, int num_samples, bool loop);
void wav_reset_stream(WaveStream *wav_stream);

#endif
