#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <string>

#include "SampleState.h"

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
		MusicData* data;
		SampleState state;
		float volume;
	};
}

#endif