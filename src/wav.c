/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "wav.h"
#include "logs.h"

#include <stdio.h>
#include <string.h>

/* TODO: HANDLE ENDIANNESS */
#define RIFF_MARKER     0x46464952
#define WAVE_MARKER     0x45564157
#define FORMAT_MARKER   0x20746d66
#define DATA_MARKER     0x61746164

#define INVALID_RIFF_MARKER(header)         (header != RIFF_MARKER)
#define INVALID_WAVE_MARKER(header)         (header != WAVE_MARKER)
#define INVALID_FORMAT_MARKER(header)       (header != FORMAT_MARKER)
#define INVALID_DATA_MARKER(header)         (header != DATA_MARKER)
#define INVALID_FORMAT_TYPE(format)         (format != 1)
#define INVALID_CHANNEL_COUNT(channelCount) (channelCount != 1 && channelCount != 2)

typedef struct riffChunk
{
    u32     riff;
    u32     fileSize;
    u32     wave;
} RiffChunk;

typedef struct formatChunk
{
    u32     marker;
    u32     size;
    u16     type;
    u16     channels;
    u32     sampleRate;
    u32     byteRate;
    u16     blockAlign;
    u16     bitsPerSample;
} FormatChunk;

typedef struct dataChunk
{
    u32     marker;
    u32     size;
} DataChunk;

typedef struct wavHeader
{
    RiffChunk   riff;
    FormatChunk format;
    DataChunk   data;
} WavHeader;

int loadWavFile(const char* filename, u8 **data, u32 *length, u8 *channelCount)
{
    WavHeader header;
    FILE *file = NULL;
    char errorMessage[256];
    memset(errorMessage, 0, sizeof(errorMessage));

    if ((file = fopen(filename, "rb")) == NULL)
    {
        sprintf(errorMessage, "Error opening file: %s.\n", filename);
        LOG_ERROR(errorMessage);
        return -1;
    }

    if (fread(&header, sizeof(WavHeader), 1, file) != 1)
    {
        sprintf(errorMessage, "Error reading file: %s.\n", filename);
        LOG_ERROR(errorMessage);
        return -1;
    }

    if (INVALID_FORMAT_TYPE(header.format.type)
        || INVALID_RIFF_MARKER(header.riff.riff)
        || INVALID_WAVE_MARKER(header.riff.wave)
        || INVALID_FORMAT_MARKER(header.format.marker)
        || INVALID_DATA_MARKER(header.data.marker))
    {
        sprintf(errorMessage, "Invalid WAV format: %s.\n", filename);
        LOG_ERROR(errorMessage);
        return -1;
    }

    if (INVALID_CHANNEL_COUNT(header.format.channels))
    {
        sprintf(errorMessage, "Invalid WAV channels: %s.\n", filename);
        LOG_ERROR(errorMessage);
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
        sprintf(errorMessage, "Invalid WAV data: %s.\n", filename);
        LOG_ERROR(errorMessage);
        return -1;
    }

    fclose(file);
    return 0;
}
