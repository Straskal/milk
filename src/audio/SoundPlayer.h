#ifndef _SOUND_PLAYER_H_
#define _SOUND_PLAYER_H_

namespace milk {
	struct SoundHandle;

	class SoundPlayer {
	public:
		virtual ~SoundPlayer() = default;

		virtual void playSound(SoundHandle* soundHandle) = 0;
		virtual void stopSound(SoundHandle* soundHandle) = 0;
	};
}

#endif