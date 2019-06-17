#ifndef _SDL_AUDIO_PLAYER_H_
#define _SDL_AUDIO_PLAYER_H_

#include "audio/AudioPlayer.h"

namespace milk {
	class SDLAudioPlayer : public AudioPlayer {
	public:
		bool init();
		void playSound(Sound* sound) override;
		void stopSound(Sound* sound) override;
		void free();
	};
}

#endif