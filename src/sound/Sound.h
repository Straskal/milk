#ifndef _SOUND_H_
#define _SOUND_H_

#include <string>

namespace milk {
	struct Sound {
		std::string path;
		void* handle;
		int refCount;
	};

	struct SoundHandle {
		Sound* sound;
		int channel;
	};
}

#endif