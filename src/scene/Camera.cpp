#include "Camera.h"

milk::Camera::Camera()
    : position_(0, 0)
{
}

void milk::Camera::position(float x, float y)
{
    position_.x = x;
    position_.y = y;
}

milk::Vector2 milk::Camera::position() const
{
    return position_;
}
