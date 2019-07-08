#ifndef _SOUND_H_
#define _SOUND_H_

#include <string>

#include "SampleState.h"
#include "data/int.h"

namespace milk
{
	struct SoundData
	{
		std::string path;
		void* handle;
		int refCount;
	};

	struct Sound
	{
		SampleState state;
		u32 uid;
		int channel;
		float volume;
	};
}

#endif