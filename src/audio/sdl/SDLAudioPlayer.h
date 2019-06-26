#ifndef _SDL_AUDIO_PLAYER_H_
#define _SDL_AUDIO_PLAYER_H_

#include "audio/AudioPlayer.h"

namespace milk
{
	class SDLAudioPlayer : public AudioPlayer
	{
	public:
		bool init();
		void free();
		void setMasterVolume(float volume) override;
		float getMasterVolume() const override;
		void playSound(Sound* sound) override;
		void stopSound(Sound* sound) override;
		bool isSoundPlaying(Sound* sound) override;
		void setSoundVolume(Sound* sound, float volume) override;
		void playMusic(Music* music, int fadeTime) override;
		void loopMusic(Music* music, int fadeTime) override;
		void stopMusic(int fadeTime) override;
		bool isMusicPlaying(const Music* music) const override;
	};
}

#endif