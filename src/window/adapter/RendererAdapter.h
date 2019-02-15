#ifndef MILK_RENDERER_ADAPTER_H
#define MILK_RENDERER_ADAPTER_H

#include "window/Renderer.h"

struct SDL_Renderer;
struct SDL_Window;

namespace milk
{
    namespace adapter
    {
        /// SDL implementation of Renderer.
        class RendererAdapter : public Renderer
        {
        public:
            static RendererAdapter& instance()
            {
                static RendererAdapter instance;
                return instance;
            }

            bool init(SDL_Window* sdlWindow, unsigned int resolutionWidth, unsigned int resolutionHeight);

            void clear(const Color& color) override;

            void drawRectangle(const Rectangle& destinationRectangle, const Color& color) override;

            void drawRectangleOutline(const Rectangle& destinationRectangle, const Color& color) override;

            void draw(const Texture& texture, const Rectangle& sourceRectangle, const Rectangle& destinationRectangle, int flipFlags) override;

            void present() override;

            Resolution resolution() const override;

            SDL_Renderer* sdlRenderer() const;

            void free();

        private:
            RendererAdapter();

            Resolution resolution_;

            SDL_Renderer* sdlRenderer_;
        };
    }
}

#endif
