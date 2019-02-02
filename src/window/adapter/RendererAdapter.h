#ifndef MILK_RENDERER_ADAPTER_H
#define MILK_RENDERER_ADAPTER_H

#include "window/Renderer.h"

struct SDL_Renderer;
struct SDL_Window;

namespace milk
{
    namespace adapter
    {
        class RendererAdapter : public Renderer
        {
        public:
            RendererAdapter();

            bool init(SDL_Window* sdlWindow,
                      unsigned int resolutionWidth,
                      unsigned int resolutionHeight);

            void clear() override;

            void drawRectangle(Rectangle& destinationRectangle, Color color) override;
            void drawRectangleOutline(Rectangle& destinationRectangle, Color color) override;

            void draw(Texture& texture,
                      Rectangle& sourceRectangle,
                      Rectangle& destinationRectangle,
                      int flipFlags) override;

            void present() override;

            Resolution resolution() const override;

            void free();

            SDL_Renderer* sdlRenderer() const;

        private:
            unsigned int resolutionWidth_;
            unsigned int resolutionHeight_;

            SDL_Renderer* sdlRenderer_;
        };
    }
}

#endif
