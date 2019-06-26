#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <string>

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
	};
}

#endif