#ifndef _SDL_AUDIO_CACHE_H_
#define _SDL_AUDIO_CACHE_H_

#include "audio/AudioCache.h"

#include <unordered_map>

namespace milk {
	class SDLAudioCache : public AudioCache {
	public:
		Sound* loadSound(const std::string& path) override;
		void dereferenceSound(Sound* texture) override;

	private:
		std::unordered_map<std::string, Sound*> m_sounds;
	};
}

#endif