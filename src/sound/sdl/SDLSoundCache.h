#ifndef _SDL_SOUND_CACHE_H_
#define _SDL_SOUND_CACHE_H_

#include "sound/SoundCache.h"

#include <unordered_map>

namespace milk {
	class SDLSoundPlayer;

	class SDLSoundCache : public SoundCache {
	public:
		SDLSoundCache(SDLSoundPlayer* soundPlayer);

		Sound* load(const std::string& path) override;
		void dereference(SoundHandle* texture) override;
		void free();

	private:
		SDLSoundPlayer* m_soundPlayer;
		std::unordered_map<std::string, Sound*> m_sounds;
	};
}

#endif