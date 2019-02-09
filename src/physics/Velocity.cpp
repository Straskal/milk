#include "Velocity.h"

const milk::ComponentType milk::Velocity::type = VELOCITY;

milk::Velocity::Velocity(Actor& actor)
        : ActorComponent(actor),
          value_(0, 0)
{
}

void milk::Velocity::value(float x, float y)
{
    value_.x = x;
    value_.y = y;
}

void milk::Velocity::value(Vector2d velocity)
{
    value_.x = velocity.x;
    value_.y = velocity.y;
}

milk::Vector2d milk::Velocity::value() const
{
    return value_;
}
