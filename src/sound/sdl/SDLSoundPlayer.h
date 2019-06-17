#ifndef _SDL_SOUND_PLAYER_H_
#define _SDL_SOUND_PLAYER_H_

#include "sound/SoundPlayer.h"

namespace milk {
	class SDLSoundPlayer : public SoundPlayer {
	public:
		bool init();
		void playSound(SoundHandle* soundHandle) override;
		void stopSound(SoundHandle* soundHandle) override;
		void free();
	};
}

#endif