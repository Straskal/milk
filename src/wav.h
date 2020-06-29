#ifndef __WAV_H__
#define __WAV_H__

#include "milk.h"

/*
 * Specification:
 * - PCM
 * - Mono or stereo
 * - 16 bit samples
 * - 44100 sample rate
 *
 * Notes:
 * Mono sounds are interleaved during mixing.
 *
 *
 * Load a wave file.
 * @param filename
 * @param data
 * @param length
 * @param channelCount
 * @return
 */
int loadWavFile(const char* filename, u8 **data, u32 *length, u8 *channelCount);

#endif
