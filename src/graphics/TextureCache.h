#ifndef _TEXTURE_CACHE_H_
#define _TEXTURE_CACHE_H_

#include <string>

namespace milk {
	struct TextureData;

	class TextureCache {
	public:
		virtual ~TextureCache() = default;

		virtual TextureData* load(const char* path) = 0;
		virtual void dereference(TextureData* textureData) = 0;
	};
}

#endif