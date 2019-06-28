#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "AudioState.h"

namespace milk
{
	struct Music;
	struct Sound;

	class AudioPlayer
	{
	public:
		virtual ~AudioPlayer() = default;

		virtual void setMasterVolume(const float volume) = 0;
		virtual float getMasterVolume() const = 0;

		virtual void playSound(Sound* sound, const float fadeTime) = 0;
		virtual void loopSound(Sound* sound, const float fadeTime) = 0;
		virtual void pauseSound(Sound* sound) = 0;
		virtual void resumeSound(Sound* sound) = 0;
		virtual void stopSound(Sound* sound, const float fadeTime) = 0;
		virtual void setSoundVolume(Sound* sound, const float volume) = 0;

		virtual void playMusic(Music* music, const float fadeTime) = 0;
		virtual void loopMusic(Music* music, const float fadeTime) = 0;
		virtual void pauseMusic(Music* music) = 0;
		virtual void resumeMusic(Music* music) = 0;
		virtual void stopMusic(Music* music, const float fadeTime) = 0;
		virtual void setMusicVolume(Music* music, const float volume) = 0;
	};
}

#endif