/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software andassociated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, andto permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice andthis permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "milk_wav.h"
#include <SDL.h>

static void _loadWav(const char* filename, SampleData *sampleData)
{
	SDL_AudioSpec waveSpec;
	SDL_LoadWAV(filename, &waveSpec, &sampleData->buffer, &sampleData->length);

    if (waveSpec.channels != 2)
    {
        SDL_AudioCVT conversion;
        SDL_BuildAudioCVT(&conversion, waveSpec.format, waveSpec.channels, waveSpec.freq, AUDIO_S16LSB, MILK_AUDIO_CHANNELS, MILK_AUDIO_FREQUENCY);
        conversion.len = sampleData->length;
        conversion.buf = (Uint8 *)SDL_malloc(conversion.len * conversion.len_mult);
        SDL_memcpy(conversion.buf, sampleData->buffer, sampleData->length);
        SDL_ConvertAudio(&conversion);
        int ratio = conversion.len_ratio;
        int newLength = conversion.len * conversion.len_ratio;
        SDL_FreeWAV(sampleData->buffer);
        sampleData->buffer = conversion.buf;
        sampleData->length = newLength;
    }
}

void milkLoadSamples(SampleData *samples)
{
	_loadWav("music.wav", samples++);
    _loadWav("punch.wav", samples++);
}

void milkFreeSamples(SampleData *samples)
{
	SDL_FreeWAV(samples->buffer);
}
