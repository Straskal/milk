#ifndef _SDL_TEXTURE_CACHE_H_
#define _SDL_TEXTURE_CACHE_H_

#include <string>
#include <unordered_map>

#include "video/TextureCache.h"

struct SDL_Renderer;

namespace milk {
	class SDLTextureCache : public TextureCache {
	public:
		SDLTextureCache();

		bool init(SDL_Renderer* rendererHandle);

		// Textures are reference counted. Every time a texture is freed or not reference from lua, dereference is called.
		Texture* load(const std::string& path) override;
		void dereference(Texture* texture) override;
		void free();

	private:
		SDL_Renderer* m_rendererHandle;
		std::unordered_map<std::string, Texture*> m_textures;
	};
}

#endif