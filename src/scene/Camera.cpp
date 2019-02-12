#include "window/adapter/RendererAdapter.h"

#include "Camera.h"

milk::Camera::Camera()
        : position_(0, 0)
{
}

void milk::Camera::position(float x, float y)
{
    position_.x = x;
    position_.y = y;

    dirty_ = true;
}

milk::Vector2 milk::Camera::position() const
{
    return position_;
}

milk::Matrix3 milk::Camera::transformation()
{
    if (dirty_)
    {
        auto resolution = adapter::RendererAdapter::instance().resolution();

        transformation_ = Matrix3::createTranslation(-position_.x, -position_.y)
                          * Matrix3::createTranslation(resolution.width * 0.5f, resolution.height * 0.5f);

        dirty_ = false;
    }

    return transformation_;
}
