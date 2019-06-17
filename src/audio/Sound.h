#ifndef _SOUND_H_
#define _SOUND_H_

#include <string>

namespace milk {
	struct SoundData {
		std::string path;
		void* handle;
		int refCount;
	};

	struct Sound {
		SoundData* data;
		int channel;
	};
}

#endif