#ifndef MILK_WINDOW_H
#define MILK_WINDOW_H

#include <string>

namespace milk
{
    class Renderer;

    /// The Game's Window.
    class Window
    {
    public:
        /// \returns the Window's width.
        virtual unsigned int width() const = 0;

        /// \returns the Window's height.
        virtual unsigned int height() const = 0;

        /// \returns true if Window is in fullscreen mode.
        virtual bool fullscreen() const = 0;

        /// Toggles fullscreen mode.
        virtual void toggleFullscreen() = 0;

        /// \returns the Window's Renderer.
        virtual Renderer& renderer() const = 0;
    };
}

#endif
