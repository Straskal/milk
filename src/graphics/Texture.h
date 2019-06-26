#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>

namespace milk
{
	struct TextureData
	{
		std::string path;
		void* handle;
		int refCount;
		int width;
		int height;
	};

	struct Texture
	{
		TextureData* data;
	};
}

#endif
