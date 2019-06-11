#ifndef MILK_TEXTURE_H
#define MILK_TEXTURE_H

#include <string>

#include "math/Vector2.h"

namespace milk {
	class Texture {
	public:
		virtual ~Texture();

		virtual std::string name() const = 0;
		virtual Vector2 size() const = 0;
		virtual void* handle() const = 0;
	};
}

#endif
