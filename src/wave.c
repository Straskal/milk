#include <stdint.h>

#include "audio.h"
#include "common.h"
#include "wave.h"

#define RIFF_MARKER_LE 0x46464952   // "RIFF"
#define WAVE_MARKER_LE 0x45564157   // "WAVE"
#define FORMAT_MARKER_LE 0x20746d66 // "fmt0"
#define DATA_MARKER_LE 0x61746164   // "data"

#define PCM 1
#define MONO 1
#define STEREO 2

#define VALID_RIFF_MARKER(header) (header == RIFF_MARKER_LE)
#define VALID_WAVE_MARKER(header) (header == WAVE_MARKER_LE)
#define VALID_FORMAT_MARKER(header) (header == FORMAT_MARKER_LE)
#define VALID_DATA_MARKER(header) (header == DATA_MARKER_LE)
#define VALID_FORMAT_TYPE(format) (format == PCM)
#define VALID_CHANNEL_COUNT(channelCount) (channelCount == MONO || channelCount == STEREO)
#define VALID_SAMPLE_SIZE(size) (size == AUDIO_BITS_PER_SAMPLE)

typedef struct
{
    uint32_t riff;
    uint32_t fileSize;
    uint32_t wave;
} RiffChunk;

typedef struct
{
    uint32_t marker;
    uint32_t size;
    uint16_t type;
    uint16_t channels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
} FormatChunk;

typedef struct
{
    uint32_t marker;
    uint32_t size;
} DataChunk;

typedef struct
{
    RiffChunk riff;
    FormatChunk format;
    DataChunk data;
} WavHeader;

static bool __readHeader(WavHeader *header, FILE *file)
{
    return fread(header, sizeof(WavHeader), 1, file) == 1 && VALID_FORMAT_TYPE(header->format.type) && VALID_RIFF_MARKER(header->riff.riff) && VALID_WAVE_MARKER(header->riff.wave) && VALID_FORMAT_MARKER(header->format.marker) && VALID_DATA_MARKER(header->data.marker) && VALID_CHANNEL_COUNT(header->format.channels) && VALID_SAMPLE_SIZE(header->format.bitsPerSample);
}

Wave *loadWave(const char *filename)
{
    FILE *file = NULL;
    WavHeader header;

    if ((file = fopen(filename, "rb")) == NULL)
        return NULL;

    if (!__readHeader(&header, file))
    {
        fclose(file);
        return NULL;
    }

    int sampleSize = header.format.channels * header.format.bitsPerSample / 8;
    int sampleCount = (int)header.data.size / sampleSize;
    int signalSize = sampleSize * sampleCount;
    int16_t *samples = (int16_t *)malloc((size_t)signalSize);

    if (fread(samples, signalSize, 1, file) != 1)
    {
        free(samples);
        fclose(file);
        return NULL;
    }

    Wave *wave = (Wave *)malloc(sizeof(Wave));
    wave->samples = samples;
    wave->sampleCount = (int)(signalSize / sizeof(int16_t));
    wave->channelCount = header.format.channels;
    fclose(file);
    return wave;
}

void freeWave(Wave *wave)
{
    free(wave->samples);
    free(wave);
}

WaveStream *openWaveStream(const char *filename)
{
    FILE *file = NULL;
    WavHeader header;

    if ((file = fopen(filename, "rb")) == NULL)
        return NULL;

    if (!__readHeader(&header, file))
    {
        fclose(file);
        return NULL;
    }

    uint32_t sampleSize = header.format.channels * header.format.bitsPerSample / 8;
    uint32_t sampleCount = header.data.size / sampleSize;
    uint32_t signalSize = sampleSize * sampleCount;

    WaveStream *waveStream = (WaveStream *)malloc(sizeof(WaveStream));
    waveStream->chunk = (int16_t *)calloc(1, AUDIO_CHUNK_SIZE);
    waveStream->file = file;
    waveStream->sampleCount = 0;
    waveStream->channelCount = header.format.channels;
    waveStream->start = ftell(file);
    waveStream->end = waveStream->start + (long)signalSize;
    return waveStream;
}

void closeWaveStream(WaveStream *waveStream)
{
    fclose(waveStream->file);
    free(waveStream->chunk);
    free(waveStream);
}

bool readWaveStream(WaveStream *waveStream, int numSamples, bool loop)
{
    long totalBytesRead = 0;
    long requestedBytes = numSamples * (long)sizeof(int16_t);
    long remainingBytes = waveStream->end - waveStream->position;
    long bytesToRead = MIN(remainingBytes, requestedBytes);

    fread(waveStream->chunk, (size_t)bytesToRead, 1, waveStream->file);

    totalBytesRead += bytesToRead;
    bool finished = ftell(waveStream->file) == waveStream->end;

    if (finished && loop)
    {
        bytesToRead = requestedBytes - remainingBytes;
        fseek(waveStream->file, waveStream->start, SEEK_SET);
        fread(waveStream->chunk + (bytesToRead / sizeof(int16_t)) + 1, (size_t)bytesToRead, 1, waveStream->file);
        totalBytesRead += bytesToRead;
    }

    waveStream->sampleCount = (int)(totalBytesRead / sizeof(int16_t));
    waveStream->position = ftell(waveStream->file);
    return finished && !loop;
}

void waveStreamSeekStart(WaveStream *waveStream)
{
    fseek(waveStream->file, waveStream->start, SEEK_SET);
    waveStream->position = waveStream->start;
}
