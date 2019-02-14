#ifndef MILK_WINDOW_ADAPTER_H
#define MILK_WINDOW_ADAPTER_H

#include <string>

#include "window/Window.h"

struct SDL_Window;

namespace milk
{
    namespace adapter
    {
        class RendererAdapter;

        /// SDL implementation of Window.
        class WindowAdapter : public Window
        {
        public:
            static WindowAdapter& instance()
            {
                static WindowAdapter instance;
                return instance;
            }

            bool init(const std::string& title, unsigned int width, unsigned int height, bool fullscreen);

            unsigned int width() const override;

            unsigned int height() const override;

            bool fullscreen() const override;

            void toggleFullscreen() override;

            SDL_Window* sdlWindow() const;

            void free();

        private:
            WindowAdapter();

            std::string title_;

            unsigned int width_;
            unsigned int height_;

            bool fullscreen_;

            SDL_Window* sdlWindow_;
        };
    }
}

#endif
