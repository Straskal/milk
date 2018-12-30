#include "Actor.h"

Actor::Actor(Scene& scene, int id, const std::string& name, const Vector2d& position)
	: scene_(scene)
	, id_(id)
	, name_(name)
	, position_(position)
	, componentBitmask_(0)
{
}

Scene& Actor::scene() const
{
	return scene_;
}

int Actor::id() const
{
	return id_;
}

std::string Actor::name() const
{
	return name_;
}

Vector2d Actor::position() const
{
	return position_;
}

void Actor::position(float x, float y)
{
	position_.x = x;
	position_.y = y;
}

void Actor::position(Vector2d position)
{
	position_.x = position.x;
	position_.y = position.y;
}
