#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

namespace milk {
	struct Music;
	struct Sound;

	class AudioPlayer {
	public:
		virtual ~AudioPlayer() = default;

		virtual void playSound(Sound* sound) = 0;
		virtual void stopSound(Sound* sound) = 0;
		virtual void playMusic(Music* music) = 0;
		virtual void stopMusic() = 0;
		virtual bool isMusicPlaying(Music* music) = 0;
	};
}

#endif