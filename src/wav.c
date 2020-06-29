#include "wav.h"
#include "logs.h"

#include <stdio.h>
#include <string.h>

// TODO: Handle endianness for wav header markers.

#define RIFF_MARKER     0x46464952
#define WAVE_MARKER     0x45564157
#define FORMAT_MARKER   0x20746d66
#define DATA_MARKER     0x61746164

#define INVALID_RIFF_MARKER(header)		(header != RIFF_MARKER)
#define INVALID_WAVE_MARKER(header)		(header != WAVE_MARKER)
#define INVALID_FORMAT_MARKER(header)	(header != FORMAT_MARKER)
#define INVALID_DATA_MARKER(header)		(header != DATA_MARKER)

// All format types other than PCM are considered invalid.
#define INVALID_FORMAT_TYPE(format) (format != 1)

// All channel counts other than 1 (mono) and 2 (stereo) are considered invalid.
#define INVALID_CHANNEL_COUNT(channelCount) (channelCount != 1 && channelCount != 2)

// The riff chunk describes the content of riff file.
typedef struct riffChunk
{
	// Will always be "RIFF"
	u32	riff;

	// The size of the entire file.
	u32	fileSize;

	// Will always be "WAVE"
	u32	wave;
} RiffChunk;

// The format chunk descibes the format of the wav data.
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
	RiffChunk   riff;
	FormatChunk format;
	DataChunk   data;
} WavHeader;

int loadWavFile(const char *filename, u8 **data, u32 *length, u8 *channelCount)
{
	FILE *file = NULL;
	WavHeader header;

	if ((file = fopen(filename, "rb")) == NULL)
	{
		LOG_ERRORF("Error opening file: %s.\n", filename);
		return -1;
	}

	if (fread(&header, sizeof(WavHeader), 1, file) != 1)
	{
		LOG_ERRORF("Error reading file: %s.\n", filename);
		return -1;
	}

	if (INVALID_FORMAT_TYPE(header.format.type)
		|| INVALID_RIFF_MARKER(header.riff.riff)
		|| INVALID_WAVE_MARKER(header.riff.wave)
		|| INVALID_FORMAT_MARKER(header.format.marker)
		|| INVALID_DATA_MARKER(header.data.marker))
	{
		LOG_ERRORF("Invalid WAV format: %s.\n", filename);
		return -1;
	}

	if (INVALID_CHANNEL_COUNT(header.format.channels))
	{
		LOG_ERRORF("Invalid WAV channels: %s.\n", filename);
		return -1;
	}

	u32 sampleSize = header.format.channels * header.format.bitsPerSample / 8;
	u32 sampleCount = header.data.size / sampleSize;
	size_t signalSize = sampleSize * sampleCount;

	*data = (u8 *)calloc(1, signalSize);
	*length = (u32)signalSize;
	*channelCount = header.format.channels;

	if (fread(*data, signalSize, 1, file) != 1)
	{
		LOG_ERRORF("Invalid WAV data: %s.\n", filename);
		return -1;
	}

	fclose(file);
	return 0;
}
