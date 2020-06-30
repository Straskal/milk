#include "wav.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// TODO: Handle endianness for wav header markers.

// "RIFF"
#define RIFF_MARKER 0x46464952

// "WAVE"
#define WAVE_MARKER 0x45564157

// "fmt0"
#define FORMAT_MARKER 0x20746d66

// "data"
#define DATA_MARKER 0x61746164

#define PCM 1
#define MONO 1
#define STEREO 2

#define INVALID_RIFF_MARKER(header) (header != RIFF_MARKER)
#define INVALID_WAVE_MARKER(header)	(header != WAVE_MARKER)
#define INVALID_FORMAT_MARKER(header)	(header != FORMAT_MARKER)
#define INVALID_DATA_MARKER(header)	(header != DATA_MARKER)
#define INVALID_FORMAT_TYPE(format) (format != PCM)

#define INVALID_CHANNEL_COUNT(channelCount) (channelCount != MONO && channelCount != STEREO)
#define INVALID_SAMPLE_SIZE(size) (size != AUDIO_BITS_PER_SAMPLE)

// The riff chunk describes the content of riff file.
typedef struct riffChunk
{
	u32	riff;
	u32	fileSize;
	u32	wave;
} RiffChunk;

// The format chunk describes the format of the wav data.
typedef struct formatChunk
{
	u32	marker;
	u32	size;
	u16	type;
	u16	channels;
	u32	sampleRate;
	u32	byteRate;
	u16	blockAlign;
	u16	bitsPerSample;
} FormatChunk;

// The data chunk describes the size of the PCM data.
typedef struct dataChunk
{
	u32	marker;
	u32	size;
} DataChunk;

typedef struct wavHeader
{
	RiffChunk riff;
	FormatChunk format;
	DataChunk data;
} WavHeader;

static bool readWavHeader(WavHeader *header, FILE *file)
{
	if (fread(header, sizeof(WavHeader), 1, file) != 1)
		return false;

	if (INVALID_FORMAT_TYPE(header->format.type) || INVALID_RIFF_MARKER(header->riff.riff)
		|| INVALID_WAVE_MARKER(header->riff.wave) || INVALID_FORMAT_MARKER(header->format.marker)
		|| INVALID_DATA_MARKER(header->data.marker) || INVALID_CHANNEL_COUNT(header->format.channels)
		|| INVALID_SAMPLE_SIZE(header->format.bitsPerSample))
		return false;

	return true;
}

int loadWavSound(SoundData *soundData, const char *filename)
{
	FILE *file = NULL;
	WavHeader header;

	if ((file = fopen(filename, "rb")) == NULL)
		return -1;

	if (!readWavHeader(&header, file))
	{
		fclose(file);
		return -1;
	}

	u32 sampleSize = header.format.channels * header.format.bitsPerSample / 8;
	u32 sampleCount = header.data.size / sampleSize;
	size_t signalSize = sampleSize * sampleCount;

	soundData->samples = (u8 *)calloc(1, signalSize);

	if (fread(soundData->samples, signalSize, 1, file) != 1)
	{
		fclose(file);
		return -1;
	}

	soundData->length = (u32)signalSize;
	soundData->channelCount = header.format.channels;

	fclose(file);
	return 0;
}

void freeWavSound(SoundData *soundData)
{
	free(soundData->samples);
}

int openWavStream(SoundStream *stream, const char *filename)
{
	FILE *file = NULL;
	WavHeader header;

	if ((file = fopen(filename, "rb")) == NULL)
		return -1;

	if (!readWavHeader(&header, file))
		return -1;

	u32 sampleSize = header.format.channels * header.format.bitsPerSample / 8;
	u32 sampleCount = header.data.size / sampleSize;
	size_t signalSize = sampleSize * sampleCount;

	stream->chunk = (u8 *)calloc(1, CHUNK_SIZE);
	stream->chunkLength = 0;
	stream->file = file;
	stream->channelCount = header.format.channels;
	stream->start = ftell(file);
	stream->end = stream->start + signalSize;

	return 1;
}

int readFromWavStream(SoundStream *stream)
{
	long start = stream->start;
	long end = stream->end;
	long remaining = MIN(end - start, CHUNK_SIZE);
	stream->chunkLength = remaining;

	return fread(stream->chunk, (size_t)remaining, 1, stream->file);
}

void closeWavStream(SoundStream *stream)
{
	if (stream->chunk != NULL)
		free(stream->chunk);

	if (stream->file != NULL)
		fclose(stream->file);
}
