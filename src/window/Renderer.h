#ifndef MILK_RENDERER_H
#define MILK_RENDERER_H

namespace milk
{
    class Rectangle;
    class Texture;

    struct Color;

    /// Renderer resolution
    struct Resolution
    {
        unsigned int width, height;
    };

    /// The Renderer is used for drawing textures to the screen.
    class Renderer
    {
    public:
        /// Clear the Renderer.
        virtual void clear(const Color& color) = 0;

        /// Draw a Rectangle.
        /// \param destinationRectangle: Rectangle's shape
        /// \param color: Rectangle's color
        virtual void drawRectangle(const Rectangle& destinationRectangle, const Color& color) = 0;

        /// Draw a Rectangle's outline.
        /// \param destinationRectangle: Rectangle's shape
        /// \param color: Rectangle's color
        virtual void drawRectangleOutline(const Rectangle& destinationRectangle, const Color& color) = 0;

        /// Draw a texture.
        /// \param texture: The Texture to draw
        /// \param sourceRectangle: The source Rectangle of the Texture
        /// \param destinationRectangle: The destination to draw the texture
        /// \param flipFlags: The Texture's flip flags
        virtual void draw(const Texture& texture, const Rectangle& sourceRectangle, const Rectangle& destinationRectangle, int flipFlags) = 0;

        /// Draw the contents of the Renderer to the Window.
        virtual void present() = 0;

        /// \returns the Renderer's resolution.
        virtual Resolution resolution() const = 0;
    };
}

#endif
