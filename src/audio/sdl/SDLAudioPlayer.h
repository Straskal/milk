#ifndef _SDL_AUDIO_PLAYER_H_
#define _SDL_AUDIO_PLAYER_H_

#include "audio/AudioPlayer.h"

namespace milk
{
	class SDLAudioPlayer : public AudioPlayer
	{
	public:
		SDLAudioPlayer();

		bool init();
		void free();

		void setMasterVolume(float volume) override;
		float getMasterVolume() const override;

		void playSound(Sound* sound, float fadeTime) override;
		void loopSound(Sound* sound, float fadeTime) override;
		void pauseSound(Sound* sound) override;
		void resumeSound(Sound* sound) override;
		void stopSound(Sound* sound, float fadeTime) override;
		void setSoundVolume(Sound* sound, float volume) override;

		void playMusic(Music* music, float fadeTime) override;
		void loopMusic(Music* music, float fadeTime) override;
		void pauseMusic(Music* music) override;
		void resumeMusic(Music* music) override;
		void stopMusic(Music* music, float fadeTime) override;
		void setMusicVolume(Music* music, float volume) override;

	private:
		float m_masterVolume;
	};
}

#endif