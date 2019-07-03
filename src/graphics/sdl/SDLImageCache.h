#ifndef _SDL_IMAGE_CACHE_H_
#define _SDL_IMAGE_CACHE_H_

#include <string>
#include <unordered_map>

#include "graphics/ImageCache.h"

struct SDL_Renderer;

namespace milk
{
	class SDLImageCache : public ImageCache
	{
	public:
		SDLImageCache();

		bool init(SDL_Renderer* rendererHandle);
		void free();

		ImageData* load(const char* path) override;
		void dereference(ImageData* imageData) override;

	private:
		SDL_Renderer* m_rendererHandle;
		std::unordered_map<std::string, ImageData*> m_images;
	};
}

#endif