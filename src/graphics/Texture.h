#ifndef MILK_TEXTURE_H
#define MILK_TEXTURE_H

struct SDL_Texture;

namespace milk
{
    /// TODO: Not sure how I'd abstract this away. After, it would ONLY be width and height.
    /// A Texture! Owns a pointer to the underlying SDL texture.
    class Texture
    {
    public:
        Texture(SDL_Texture* sdlTexture, int width, int height);

        ~Texture();

        /// \returns et the raw SDL_Texture.
        SDL_Texture* get() const;

        /// \returns the Texture's width.
        int width() const;

        /// \returns the Texture's height.
        int height() const;

    private:
        SDL_Texture* sdlTexture_;

        int width_;
        int height_;
    };
}

#endif
