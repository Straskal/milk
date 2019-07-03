#ifndef _SOUND_H_
#define _SOUND_H_

#include <string>

#include "SampleState.h"

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
		SoundData* data;
		SampleState state;
		int channel;
		float volume;
	};
}

#endif