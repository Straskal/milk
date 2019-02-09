#ifndef MILK_CAMERA_H
#define MILK_CAMERA_H

#include "math/Vector2d.h"

namespace milk
{
    class Actor;

    /// The Scene's Camera. The Camera only has a position because its size always matches the games virtual resolution.
    class Camera
    {
    public:
        Camera();

        /// Set the cameras position.
        /// \param x: x coordinate
        /// \param y: y coordinate
        void position(float x, float y);

        /// Get the Camera's position.
        /// \return: Cameras position.
        Vector2d position() const;

    private:
        Vector2d position_;
    };
}

#endif
