#include "Actor.h"

#include "ActorComponent.h"

milk::Actor::Actor(Scene& scene, int id, const std::string& name, const Vector2& position)
        : scene_(scene),
          id_(id),
          name_(name),
          position_(position),
          componentBitmask_(0)
{
}

milk::Actor::~Actor() = default;

milk::Scene& milk::Actor::scene() const
{
    return scene_;
}

int milk::Actor::id() const
{
    return id_;
}

std::string milk::Actor::name() const
{
    return name_;
}

milk::Vector2 milk::Actor::position() const
{
    return position_;
}

void milk::Actor::position(float x, float y)
{
    position_.x = x;
    position_.y = y;
}
