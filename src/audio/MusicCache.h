#ifndef _MUSIC_CACHE_H_
#define _MUSIC_CACHE_H_

#include <string>

namespace milk {
	struct MusicData;

	class MusicCache {
	public:
		virtual ~MusicCache() = default;

		virtual MusicData* load(const std::string& path) = 0;
		virtual void dereference(MusicData* musicData) = 0;
	};
}

#endif