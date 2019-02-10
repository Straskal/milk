#ifndef MILK_VELOCITY_H
#define MILK_VELOCITY_H

#include "scene/ActorComponent.h"

#include "math/Vector2.h"

namespace milk
{
    /// All objects that move should contain a Velocity component.
    /// In regards to movement, directly setting an objects position is ill advised as the collision system operates on Velocity.
    class Velocity : public ActorComponent
    {
    public:
        static const ComponentType type;

        explicit Velocity(Actor& actor);

        /// Sets the velocity.
        void value(float x, float y);

        /// Sets the velocity.
        void value(Vector2 velocity);

        /// \returns the velocity.
        Vector2 value() const;

    private:
        Vector2 value_;
    };
}

#endif
