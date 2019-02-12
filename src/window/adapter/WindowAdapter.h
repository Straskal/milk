#ifndef MILK_WINDOW_ADAPTER_H
#define MILK_WINDOW_ADAPTER_H

#include <memory>
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

            ~WindowAdapter();

            bool init(const std::string& title,
                      unsigned int width,
                      unsigned int height,
                      unsigned int resolutionWidth,
                      unsigned int resolutionHeight,
                      bool fullscreen);

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

            unsigned int nativeWidth_;
            unsigned int nativeHeight_;

            bool fullscreen_;

            SDL_Window* sdlWindow_;
        };
    }
}

#endif
