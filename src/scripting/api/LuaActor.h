#ifndef MILK_LUA_ACTOR_H
#define MILK_LUA_ACTOR_H

#include <string>

#include "math/Vector2d.h"

namespace milk
{
    class Actor;

    namespace lua
    {
        namespace actor
        {
            std::string name(Actor& actor);

            Vector2d position(Actor& actor);

            void move(Actor& actor, float x, float y);

            void setAnimation(Actor& actor, const std::string& name);

            void flipX(Actor& actor);

            void flipY(Actor& actor);
        }
    }
}


#endif
