#ifndef _SOUND_CACHE_H_
#define _SOUND_CACHE_H_

#include <string>

namespace milk {
	struct Sound;
	struct SoundHandle;

	class SoundCache {
	public:
		virtual ~SoundCache() = default;

		virtual Sound* load(const std::string& path) = 0;
		virtual void dereference(SoundHandle* texture) = 0;
	};
}

#endif