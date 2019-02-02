#ifndef MILK_RENDERER_H
#define MILK_RENDERER_H

#include "graphics/Color.h"

namespace milk
{
    class Rectangle;
    class Texture;

    /// Renderer resolution
    struct Resolution
    {
        int width, height;
    };

    /// The Renderer is used for drawing textures to the screen
    class Renderer
    {
    public:
        /// Clear the renderer to black.
        virtual void clear() = 0;

        /// Draw a Rectangle
        /// \param destinationRectangle: Rectangle's shape
        /// \param color: Rectangle's color
        virtual void drawRectangle(Rectangle& destinationRectangle, Color color) = 0;

        /// Draw a Rectangle's outline
        /// \param destinationRectangle: Rectangle's shape
        /// \param color: Rectangle's color
        virtual void drawRectangleOutline(Rectangle& destinationRectangle, Color color) = 0;

        /// Draw the texture
        /// \param texture: The Texture to draw
        /// \param sourceRectangle: The source Rectangle of the Texture
        /// \param destinationRectangle: The destination to draw the texture
        /// \param flipFlags: The Texture's flip flags
        virtual void draw(Texture& texture,
                          Rectangle& sourceRectangle,
                          Rectangle& destinationRectangle,
                          int flipFlags) = 0;

        /// Draw the contents of the renderer to the window.
        virtual void present() = 0;

        /// \returns the Renderer's resolution
        virtual Resolution resolution() const = 0;
    };
}

#endif
