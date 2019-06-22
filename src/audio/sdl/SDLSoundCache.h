#ifndef _SDL_SOUND_CACHE_H_
#define _SDL_SOUND_CACHE_H_

#include "audio/SoundCache.h"

#include <string>
#include <unordered_map>

namespace milk {
	class SDLSoundCache : public SoundCache {
	public:
		SoundData* load(const char* path) override;
		void dereference(SoundData* soundData) override;
		void free();

	private:
		std::unordered_map<std::string, SoundData*> m_sounds;
	};
}

#endif