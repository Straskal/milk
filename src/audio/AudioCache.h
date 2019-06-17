#ifndef _AUDIO_CACHE_H_
#define _AUDIO_CACHE_H_

#include <string>

namespace milk {
	struct Sound;

	class AudioCache {
	public:
		virtual ~AudioCache() = default;

		virtual Sound* loadSound(const std::string& path) = 0;
		virtual void dereferenceSound(Sound* texture) = 0;
	};
}

#endif