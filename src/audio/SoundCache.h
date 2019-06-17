#ifndef _SOUND_CACHE_H_
#define _SOUND_CACHE_H_

#include <string>

namespace milk {
	struct Sound;

	class SoundCache {
	public:
		virtual ~SoundCache() = default;

		virtual Sound* load(const std::string& path) = 0;
		virtual void dereference(Sound* texture) = 0;
	};
}

#endif