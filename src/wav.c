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

#include <stdio.h>


#define PCM 1
#define READ_CHUNK(file, chunk, type) (fread(chunk, sizeof(type), 1, file) != 1)
#define IS_MONO(channels) (channels == 1)


typedef struct riffChunk
{
    u8      riff[4];
    u32     fileSize;
    u8      wave[4];
} RiffChunk;


typedef struct formatChunk
{
    u8      marker[4];
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
    u8      marker[4];
    u32     size;
} DataChunk;


typedef struct wavHeader
{
    RiffChunk   riff;
    FormatChunk format;
    DataChunk   data;
} WavHeader;


/* Take a mono sound and interleave it between L and R stereo channels. */
static void monoToStereo(const u8 *monoData, u8 *stereoData, int sampleCount)
{
    for (int i = 0; i < sampleCount; i += 2)
    {
        stereoData[i * 2] = monoData[i];
        stereoData[i * 2 + 1] = monoData[i + 1];
        stereoData[i * 2 + 2] = monoData[i];
        stereoData[i * 2 + 3] = monoData[i + 1];
    }
}


int loadWavFile(const char* filename, u8 **data, u32 *length)
{
    WavHeader header;
    FILE *file = NULL;

    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("Could not open file\n");
        return -1;
    }

    if (READ_CHUNK(file, &header.riff, RiffChunk))
    {
        printf("Error reading riff chunk.");
        return -1;
    }

    if (READ_CHUNK(file, &header.format, FormatChunk))
    {
        printf("Error reading riff chunk.");
        return -1;
    }

    if (header.format.type != PCM)
    {
        printf("Wav format type must be PCM.");
        return -1;
    }

    if (READ_CHUNK(file, &header.data, DataChunk))
    {
        printf("Error reading data chunk.");
        return -1;
    }

    u32 sampleSize = header.format.channels * header.format.bitsPerSample / 8;
    u32 sampleCount = header.data.size / sampleSize;
    size_t signalSize = sampleSize * sampleCount;

    *data = (u8 *)calloc(1, signalSize);
    *length = (u32)signalSize;

    if (fread(*data, signalSize, 1, file) != 1)
    {
        printf("Error reading wav data.");
        return -1;
    }

    if (IS_MONO(header.format.channels))
    {
        u8 *converted = (u8 *)calloc(1, signalSize * 2);
        monoToStereo(*data, converted, sampleCount);
        free(*data);
        *data = converted;
        *length *= 2;
    }

    fclose(file);
    return 0;
}
