#ifndef _SOUND_CACHE_H_
#define _SOUND_CACHE_H_

#include <string>

namespace milk {
	struct SoundData;

	class SoundCache {
	public:
		virtual ~SoundCache() = default;

		virtual SoundData* load(const std::string& path) = 0;
		virtual void dereference(SoundData* soundData) = 0;
	};
}

#endif