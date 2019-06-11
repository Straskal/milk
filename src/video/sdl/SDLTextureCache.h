#ifndef MILK_SDL_TEXTURE_CACHE_H
#define MILK_SDL_TEXTURE_CACHE_H

#include <string>
#include <unordered_map>

#include "video/TextureCache.h"

struct SDL_Renderer;

namespace milk {
	class SDLTextureCache : public TextureCache {
	public:
		SDLTextureCache();

		bool init(SDL_Renderer* rendererHandle);
		Texture* reference(const std::string& path) override;
		void dereference(Texture* texture) override;
		void free();

	private:
		SDL_Renderer* m_rendererHandle;
		std::unordered_map<std::string, Texture*> m_textures;
	};
}

#endif