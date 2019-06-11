#ifndef MILK_TEXTURE_CACHE_H
#define MILK_TEXTURE_CACHE_H

#include <string>

namespace milk {
	class Texture;

	class TextureCache {
	public:
		virtual Texture* reference(const std::string& path) = 0;
		virtual void dereference(const std::string& path) = 0;
	};
}

#endif