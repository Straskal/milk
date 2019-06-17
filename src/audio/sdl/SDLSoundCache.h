#ifndef _SDL_SOUND_CACHE_H_
#define _SDL_SOUND_CACHE_H_

#include "audio/SoundCache.h"

#include <unordered_map>

namespace milk {
	class SDLAudioPlayer;

	class SDLSoundCache : public SoundCache {
	public:
		SDLSoundCache(SDLAudioPlayer* audioPlayer);

		Sound* load(const std::string& path) override;
		void dereference(SoundHandle* texture) override;
		void free();

	private:
		SDLAudioPlayer* m_audioPlayer;
		std::unordered_map<std::string, Sound*> m_sounds;
	};
}

#endif