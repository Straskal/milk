#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

namespace milk {
	struct Sound;

	class AudioPlayer {
	public:
		virtual ~AudioPlayer() = default;

		virtual void playSound(Sound* sound) = 0;
	};
}

#endif