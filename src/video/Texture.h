#ifndef MILK_TEXTURE_H
#define MILK_TEXTURE_H

struct SDL_Texture;

namespace milk {
	class Texture {
	public:
		Texture(SDL_Texture* sdlTexture, int width, int height);
		~Texture();

		SDL_Texture* get() const;
		int width() const;
		int height() const;

	private:
		SDL_Texture* m_handle;
		int m_width;
		int m_height;
	};
}

#endif
