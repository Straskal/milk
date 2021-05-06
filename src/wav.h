#ifndef __WAVE_H__
#define __WAVE_H__

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    int16_t *samples;
    int channelCount;
    int sampleCount;
} Wav;

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

// Loads an entire wav audio into memory.
// Wav are good for short burst sounds, since they are usually smaller in size.
Wav *loadWave(const char *filename);

// Frees a wav.
void freeWave(Wav *wave);

// Opens a wav stream.
// Wav streams are good for music, since music files are usually larger in size.
// We don't keep the whole thing loaded. We read and cache chunks at a time.
WavStream *openWaveStream(const char *filename);

// Closes a wav stream and releases the file.
void closeWaveStream(WavStream *waveStream);

// Reads n samples from a wav stream.
// If loop is true, then the stream will wrap back to the start of the audio.
bool readWaveStream(WavStream *waveStream, int numSamples, bool loop);

// Resets a stream to its starting position.
void waveStreamSeekStart(WavStream *waveStream);

#endif
