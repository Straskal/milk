#ifndef MILK_TEXTURE_H
#define MILK_TEXTURE_H

#include <string>

#include "math/Vector2.h"

namespace milk {
	struct Texture {
		std::string path;
		void* handle;
		int refCount;
		int width;
		int height;
	};

	struct TextureHandle {
		Texture* texture;
	};
}

#endif
