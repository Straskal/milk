#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <string>

#include "data/int.h"

namespace milk
{
	enum class SampleState
	{
		PLAYING,
		PAUSED,
		STOPPED
	};

	struct SoundData
	{
		std::string path;
		void* handle;
		int refCount;
	};

	struct Sound
	{
		u32 uid;
		SampleState state;
		int channel;
		float volume;
	};

	struct MusicData
	{
		std::string path;
		void* handle;
		int refCount;
	};

	struct Music
	{
		u32 uid;
		SampleState state;
		float volume;
	};
}

#endif