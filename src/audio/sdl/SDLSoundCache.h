#ifndef _SDL_SOUND_CACHE_H_
#define _SDL_SOUND_CACHE_H_

#include "audio/SoundCache.h"

#include <unordered_map>

namespace milk {
	class SDLAudioPlayer;

	class SDLSoundCache : public SoundCache {
	public:
		Sound* load(const std::string& path) override;
		void dereference(Sound* sound) override;
		void free();

	private:
		std::unordered_map<std::string, Sound*> m_sounds;
	};
}

#endif