#ifndef _SDL_AUDIO_PLAYER_H_
#define _SDL_AUDIO_PLAYER_H_

#include "audio/AudioPlayer.h"
#include "audio/AudioState.h"

namespace milk
{
	class SDLAudioPlayer : public AudioPlayer
	{
	public:
		SDLAudioPlayer();

		bool init();
		void free();

		void setMasterVolume(const float volume) override;
		float getMasterVolume() const override;

		void playSound(Sound* sound, const float fadeTime) override;
		void loopSound(Sound* sound, const float fadeTime) override;
		void pauseSound(Sound* sound) override;
		void resumeSound(Sound* sound) override;
		void stopSound(Sound* sound, const float fadeTime) override;
		void setSoundVolume(Sound* sound, const float volume) override;

		void playMusic(Music* music, const float fadeTime) override;
		void loopMusic(Music* music, const float fadeTime) override;
		void pauseMusic(Music* music) override;
		void resumeMusic(Music* music) override;
		void stopMusic(Music* music, const float fadeTime) override;
		void setMusicVolume(Music* music, const float volume) override;

	private:
		float m_masterVolume;
	};
}

#endif