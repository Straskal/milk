#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <string>

#include "SampleState.h"
#include "data/int.h"

namespace milk
{
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