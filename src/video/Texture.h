#ifndef _TEXTURE_H_
#define _TEXTURE_H_

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
