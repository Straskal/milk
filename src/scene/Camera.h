#ifndef MILK_CAMERA_H
#define MILK_CAMERA_H

#include "math/Matrix3.h"
#include "math/Vector2.h"

namespace milk
{
    class Actor;

    /// A Camera! The Camera only has a position because its size always matches the games resolution.
    class Camera
    {
    public:
        Camera();

        /// Set the Cameras position.
        /// \param x: x coordinate
        /// \param y: y coordinate
        void position(float x, float y);

        /// \returns the Cameras position.
        Vector2 position() const;

        /// \returns the Cameras matrix transformation
        Matrix3 transformation();

    private:
        bool dirty_;

        Vector2 position_;

        Matrix3 transformation_;
    };
}

#endif
