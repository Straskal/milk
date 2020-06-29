#ifndef __WAV_H__
#define __WAV_H__

#include "milk.h"

// There are limits to what kind of wave files we can load.
// - Mono and stereo PCM
// - Signed 16 bit samples
// - 44,100 sample rate.
int loadWavFile(const char* filename, u8 **data, u32 *length, u8 *channelCount);

#endif
