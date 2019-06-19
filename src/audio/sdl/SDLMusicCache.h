#ifndef _SDL_MUSIC_CACHE_H_
#define _SDL_MUSIC_CACHE_H_

#include "audio/MusicCache.h"

#include <unordered_map>

namespace milk {
	class SDLMusicCache : public MusicCache {
	public:
		MusicData* load(const std::string& path) override;
		void dereference(MusicData* musicData) override;
		void free();

	private:
		std::unordered_map<std::string, MusicData*> m_music;
	};
}

#endif