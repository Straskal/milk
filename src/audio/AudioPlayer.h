#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

namespace milk
{
	struct Music;
	struct Sound;

	class AudioPlayer
	{
	public:
		virtual ~AudioPlayer() = default;

		virtual void setMasterVolume(float volume) = 0;
		virtual float getMasterVolume() const = 0;

		virtual void playSound(Sound* sound, float fadeTime) = 0;
		virtual void loopSound(Sound* sound, float fadeTime) = 0;
		virtual void pauseSound(Sound* sound) = 0;
		virtual void resumeSound(Sound* sound) = 0;
		virtual void stopSound(Sound* sound, float fadeTime) = 0;
		virtual void setSoundVolume(Sound* sound, float volume) = 0;

		virtual void playMusic(Music* music, float fadeTime) = 0;
		virtual void loopMusic(Music* music, float fadeTime) = 0;
		virtual void pauseMusic(Music* music) = 0;
		virtual void resumeMusic(Music* music) = 0;
		virtual void stopMusic(Music* music, float fadeTime) = 0;
		virtual void setMusicVolume(Music* music, float volume) = 0;
	};
}

#endif