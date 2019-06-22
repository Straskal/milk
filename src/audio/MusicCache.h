#ifndef _MUSIC_CACHE_H_
#define _MUSIC_CACHE_H_

namespace milk {
	struct MusicData;

	class MusicCache {
	public:
		virtual ~MusicCache() = default;

		virtual MusicData* load(const char* path) = 0;
		virtual void dereference(MusicData* musicData) = 0;
	};
}

#endif