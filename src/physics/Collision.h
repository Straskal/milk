#ifndef MILK_COLLISIONEVENT_H
#define MILK_COLLISIONEVENT_H

#include "math/Rectangle.h"

namespace milk
{
    class BoxCollider;

    enum class CollisionSide
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };

    /// A Collision in the Physics world
    struct Collision
    {
        /// \param actor: The Actor colliding with another actor
        /// \param other: The other Actor
        /// \param direction: The direction in which the two actors collided with eachother, from the perspective of the main actor
        /// \param depth: The collision intersection depth
        Collision(BoxCollider& actor, BoxCollider& other, CollisionSide direction, Rectangle depth)
                : actor(actor),
                  other(other),
                  direction(direction),
                  depthRect(depth)
        {
        }

        BoxCollider& actor;
        BoxCollider& other;
        CollisionSide direction;
        Rectangle depthRect;
    };
}

#endif
