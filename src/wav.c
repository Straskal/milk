#include <stdint.h>

#include "common.h"
#include "wav.h"

#define RIFF_MARKER_LE 0x46464952 // "RIFF"
#define WAVE_MARKER_LE 0x45564157 // "WAVE"
#define FORMAT_MARKER_LE 0x20746d66  // "fmt0"
#define DATA_MARKER_LE 0x61746164    // "data"

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

typedef struct {
  uint32_t riff;
  uint32_t fileSize;
  uint32_t wave;
} RiffChunk;

typedef struct {
  uint32_t marker;
  uint32_t size;
  uint16_t type;
  uint16_t channels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
} FormatChunk;

typedef struct {
  uint32_t marker;
  uint32_t size;
} DataChunk;

typedef struct {
  RiffChunk riff;
  FormatChunk format;
  DataChunk data;
} WavHeader;

static bool readWavHeader(WavHeader *header, FILE *file) {
  return fread(header, sizeof(WavHeader), 1, file) == 1 && VALID_FORMAT_TYPE(header->format.type)
      && VALID_RIFF_MARKER(header->riff.riff) && VALID_WAVE_MARKER(header->riff.wave)
      && VALID_FORMAT_MARKER(header->format.marker) && VALID_DATA_MARKER(header->data.marker)
      && VALID_CHANNEL_COUNT(header->format.channels) && VALID_SAMPLE_SIZE(header->format.bitsPerSample);
}

bool loadWavSound(SoundData *soundData, const char *filename) {
  FILE *file = NULL;
  WavHeader header;
  if ((file = fopen(filename, "rb")) == NULL)
    return -1;
  if (!readWavHeader(&header, file)) {
    fclose(file);
    return -1;
  }
  int sampleSize = header.format.channels * header.format.bitsPerSample / 8;
  int sampleCount = (int)header.data.size / sampleSize;
  int signalSize = sampleSize * sampleCount;
  int16_t *samples = (int16_t *) calloc(1, (size_t)signalSize);
  if (fread(samples, signalSize, 1, file) != 1) {
    free(samples);
    fclose(file);
    return false;
  }
  soundData->samples = samples;
  soundData->sampleCount = (int) (signalSize / sizeof(int16_t));
  soundData->channelCount = header.format.channels;
  fclose(file);
  return true;
}

void freeWavSound(SoundData *soundData) {
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
  if (!readWavHeader(&header, file)) {
    fclose(file);
    return false;
  }
  uint32_t sampleSize = header.format.channels * header.format.bitsPerSample / 8;
  uint32_t sampleCount = header.data.size / sampleSize;
  uint32_t signalSize = sampleSize * sampleCount;
  streamData->file = file;
  streamData->chunk = (int16_t *) calloc(1, AUDIO_CHUNK_SIZE);
  streamData->sampleCount = 0;
  streamData->channelCount = header.format.channels;
  streamData->start = ftell(file);
  streamData->end = streamData->start + (long) signalSize;
  return true;
}

void closeWavStream(SoundStreamData *streamData)
{
  free(streamData->chunk);
  fclose(streamData->file);
  streamData->file = NULL;
  streamData->chunk = NULL;
  streamData->sampleCount = 0;
  streamData->channelCount = 0;
  streamData->position = 0;
  streamData->start = 0;
  streamData->end = 0;
}

bool wavStreamRead(SoundStreamData *streamData, int numSamples, bool loop) {
  long totalBytesRead = 0;
  long requestedBytes = numSamples * (long)sizeof(int16_t);
  long remainingBytes = streamData->end - streamData->position;
  long bytesToRead = MIN(remainingBytes, requestedBytes);
  fread(streamData->chunk, (size_t)bytesToRead, 1, streamData->file);
  totalBytesRead += bytesToRead;
  bool finished = ftell(streamData->file) == streamData->end;
  if (finished && loop) {
    bytesToRead = requestedBytes - remainingBytes;
    fseek(streamData->file, streamData->start, SEEK_SET);
    fread(streamData->chunk + (bytesToRead / sizeof(int16_t)) + 1, (size_t)bytesToRead, 1, streamData->file);
    totalBytesRead += bytesToRead;
  }
  streamData->sampleCount = (int) (totalBytesRead / sizeof(int16_t));
  streamData->position = ftell(streamData->file);
  return finished && !loop;
}

void wavStreamSeekStart(SoundStreamData *streamData) {
  fseek(streamData->file, streamData->start, SEEK_SET);
  streamData->position = streamData->start;
}
