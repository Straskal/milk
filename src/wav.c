#include "wav.h"

#define RIFF_MARKER_LE 0x46464952 // "RIFF"
#define WAVE_MARKER_LE 0x45564157 // "WAVE"
#define FORMAT_MARKER 0x20746d66  // "fmt0"
#define DATA_MARKER 0x61746164    // "data"

#define PCM     1
#define MONO    1
#define STEREO  2

#define INVALID_RIFF_MARKER(header) (header != RIFF_MARKER_LE)
#define INVALID_WAVE_MARKER(header)  (header != WAVE_MARKER_LE)
#define INVALID_FORMAT_MARKER(header)  (header != FORMAT_MARKER)
#define INVALID_DATA_MARKER(header)  (header != DATA_MARKER)
#define INVALID_FORMAT_TYPE(format) (format != PCM)

#define INVALID_CHANNEL_COUNT(channelCount) (channelCount != MONO && channelCount != STEREO)
#define INVALID_SAMPLE_SIZE(size) (size != AUDIO_BITS_PER_SAMPLE)

// The riff chunk describes the content of riff file.
typedef struct riffChunk
{
  u32 riff;
  u32 fileSize;
  u32 wave;
} RiffChunk;

// The format chunk describes the format of the wav data.
typedef struct formatChunk
{
  u32 marker;
  u32 size;
  u16 type;
  u16 channels;
  u32 sampleRate;
  u32 byteRate;
  u16 blockAlign;
  u16 bitsPerSample;
} FormatChunk;

// The data chunk describes the size of the PCM data.
typedef struct dataChunk
{
  u32 marker;
  u32 size;
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

bool loadWavSound(SoundData *soundData, const char *filename)
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

  int sampleSize = header.format.channels * header.format.bitsPerSample / 8;
  int sampleCount = (int)header.data.size / sampleSize;
  int signalSize = sampleSize * sampleCount;
  s16 *samples = (s16 *) calloc(1, (size_t)signalSize);

  if (fread(samples, signalSize, 1, file) != 1)
  {
    free(samples);
    fclose(file);
    return false;
  }

  soundData->samples = samples;
  soundData->sampleCount = (int) (signalSize / sizeof(s16));
  soundData->channelCount = header.format.channels;

  fclose(file);
  return true;
}

void freeWavSound(SoundData *soundData)
{
  if (soundData->samples != NULL)
    free(soundData->samples);

  soundData->samples = NULL;
  soundData->sampleCount = 0;
  soundData->channelCount = 0;
}

bool openWavStream(SoundStreamData *streamData, const char *filename)
{
  FILE *file = NULL;
  WavHeader header;

  if ((file = fopen(filename, "rb")) == NULL)
    return false;

  if (!readWavHeader(&header, file))
  {
    fclose(file);
    return false;
  }

  u32 sampleSize = header.format.channels * header.format.bitsPerSample / 8;
  u32 sampleCount = header.data.size / sampleSize;
  u32 signalSize = sampleSize * sampleCount;
  streamData->file = file;
  streamData->chunk = (s16 *) calloc(1, AUDIO_CHUNK_SIZE);
  streamData->sampleCount = 0;
  streamData->channelCount = header.format.channels;
  streamData->start = ftell(file);
  streamData->end = streamData->start + (long) signalSize;
  return true;
}

void closeWavStream(SoundStreamData *streamData)
{
  if (streamData->chunk != NULL)
    free(streamData->chunk);

  if (streamData->file != NULL)
    fclose(streamData->file);

  streamData->file = NULL;
  streamData->chunk = NULL;
  streamData->sampleCount = 0;
  streamData->channelCount = 0;
  streamData->start = 0;
  streamData->end = 0;
}

bool readFromWavStream(SoundStreamData *streamData, int numSamples, bool loop)
{
  long totalBytesRead = 0;
  long requestedBytes = numSamples * (long)sizeof(s16);
  long remainingBytes = streamData->end - streamData->position;
  long bytesToRead = MIN(remainingBytes, requestedBytes);
  fread(streamData->chunk, (size_t)bytesToRead, 1, streamData->file);
  totalBytesRead += bytesToRead;
  bool finished = ftell(streamData->file) == streamData->end;

  if (finished && loop)
  {
    bytesToRead = requestedBytes - remainingBytes;
    fseek(streamData->file, streamData->start, SEEK_SET);
    fread(streamData->chunk + (bytesToRead / sizeof(s16)) + 1, (size_t)bytesToRead, 1, streamData->file);
    totalBytesRead += bytesToRead;
  }

  streamData->sampleCount = (int) (totalBytesRead / sizeof(s16));
  streamData->position = ftell(streamData->file);
  return finished && !loop;
}

void moveWavStreamToStart(SoundStreamData *streamData)
{
  fseek(streamData->file, streamData->start, SEEK_SET);
  streamData->position = streamData->start;
}
