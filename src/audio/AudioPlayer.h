#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

namespace milk {
	struct SoundHandle;

	class AudioPlayer {
	public:
		virtual ~AudioPlayer() = default;

		virtual void playSound(SoundHandle* soundHandle) = 0;
		virtual void stopSound(SoundHandle* soundHandle) = 0;
	};
}

#endif