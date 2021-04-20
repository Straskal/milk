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
    uint32_t file_size;
    uint32_t wave;
} RiffChunk;

typedef struct
{
    uint32_t marker;
    uint32_t size;
    uint16_t type;
    uint16_t channel_count;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
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

static bool read_header(WavHeader *header, FILE *file)
{
    return fread(header, sizeof(WavHeader), 1, file) == 1
        && VALID_FORMAT_TYPE(header->format.type)
        && VALID_RIFF_MARKER(header->riff.riff)
        && VALID_WAVE_MARKER(header->riff.wave)
        && VALID_FORMAT_MARKER(header->format.marker)
        && VALID_DATA_MARKER(header->data.marker)
        && VALID_CHANNEL_COUNT(header->format.channel_count)
        && VALID_SAMPLE_SIZE(header->format.bits_per_sample);
}

Wave *wav_load(const char *filename)
{
    FILE *file = NULL;
    WavHeader header;

    if ((file = fopen(filename, "rb")) == NULL)
    {
        return NULL;
    }

    if (!read_header(&header, file))
    {
        fclose(file);
        return NULL;
    }

    int sample_size_in_bytes = header.format.channel_count * header.format.bits_per_sample / 8;
    int sample_count = (int)header.data.size / sample_size_in_bytes;
    int signal_size = sample_size_in_bytes * sample_count;
    int16_t *samples = (int16_t *)malloc((size_t)signal_size);

    if (fread(samples, signal_size, 1, file) != 1)
    {
        free(samples);
        fclose(file);
        return NULL;
    }

    Wave *wave = (Wave *)malloc(sizeof(Wave));
    wave->samples = samples;
    wave->sample_count = (int)(signal_size / sizeof(int16_t));
    wave->channel_count = header.format.channel_count;
    fclose(file);

    return wave;
}

void wav_free(Wave *wave)
{
    free(wave->samples);
    free(wave);
}

WaveStream *wav_open_stream(const char *filename)
{
    FILE *file = NULL;
    WavHeader header;

    if ((file = fopen(filename, "rb")) == NULL)
    {
        return NULL;
    }

    if (!read_header(&header, file))
    {
        fclose(file);
        return NULL;
    }

    uint32_t sample_size_in_bytes = header.format.channel_count * header.format.bits_per_sample / 8;
    uint32_t sample_count = header.data.size / sample_size_in_bytes;
    uint32_t signal_size = sample_size_in_bytes * sample_count;

    WaveStream *wav_stream = (WaveStream *)malloc(sizeof(WaveStream));
    wav_stream->chunk = (int16_t *)calloc(1, AUDIO_CHUNK_SIZE);
    wav_stream->file = file;
    wav_stream->sample_count = 0;
    wav_stream->channel_count = header.format.channel_count;
    wav_stream->start = ftell(file);
    wav_stream->end = wav_stream->start + (long)signal_size;

    return wav_stream;
}

void wav_close_stream(WaveStream *wav_stream)
{
    fclose(wav_stream->file);
    free(wav_stream->chunk);
    free(wav_stream);
}

bool wav_read_stream(WaveStream *wav_stream, int num_samples, bool loop)
{
    long total_bytes_read = 0;
    long num_requested_bytes = num_samples * (long)sizeof(int16_t);
    long num_remaining_bytes = wav_stream->end - wav_stream->position;
    long num_bytes_to_read = MIN(num_remaining_bytes, num_requested_bytes);

    fread(wav_stream->chunk, (size_t)num_bytes_to_read, 1, wav_stream->file);

    total_bytes_read += num_bytes_to_read;
    bool finished = ftell(wav_stream->file) == wav_stream->end;

    if (finished && loop)
    {
        num_bytes_to_read = num_requested_bytes - num_remaining_bytes;
        fseek(wav_stream->file, wav_stream->start, SEEK_SET);
        fread(wav_stream->chunk + (num_bytes_to_read / sizeof(int16_t)) + 1, (size_t)num_bytes_to_read, 1, wav_stream->file);
        total_bytes_read += num_bytes_to_read;
    }

    wav_stream->sample_count = (int)(total_bytes_read / sizeof(int16_t));
    wav_stream->position = ftell(wav_stream->file);

    return finished && !loop;
}

void wav_reset_stream(WaveStream *wav_stream)
{
    fseek(wav_stream->file, wav_stream->start, SEEK_SET);
    wav_stream->position = wav_stream->start;
}
