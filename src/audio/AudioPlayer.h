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
		virtual void playSound(Sound* sound) = 0;
		virtual void stopSound(Sound* sound) = 0;
		virtual void playFafSound(Sound* sound) = 0;
		virtual bool isSoundPlaying(Sound* sound) = 0;
		virtual void setSoundVolume(Sound* sound, float volume) = 0;
		virtual void playMusic(Music* music, int fadeTime) = 0;
		virtual void loopMusic(Music* music, int fadeTime) = 0;
		virtual void stopMusic(int fadeTime) = 0;
		virtual bool isMusicPlaying(const Music* music) const = 0;
	};
}

#endif