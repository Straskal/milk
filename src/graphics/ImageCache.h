#ifndef _IMAGE_CACHE_H_
#define _IMAGE_CACHE_H_

#include <string>

namespace milk
{
	struct ImageData;

	class ImageCache
	{
	public:
		virtual ~ImageCache() = default;

		virtual ImageData* load(const char* path) = 0;
		virtual void dereference(ImageData* imageData) = 0;
	};
}

#endif