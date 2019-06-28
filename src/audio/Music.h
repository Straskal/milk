#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <string>

#include "AudioState.h"

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
		AudioState state;
		float volume;
	};
}

#endif