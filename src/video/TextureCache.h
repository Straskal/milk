#ifndef _TEXTURE_CACHE_H_
#define _TEXTURE_CACHE_H_

#include <string>

namespace milk {
	struct Texture;

	class TextureCache {
	public:
		virtual ~TextureCache() = default;

		virtual Texture* load(const std::string& path) = 0;
		virtual void dereference(Texture* texture) = 0;
	};
}

#endif