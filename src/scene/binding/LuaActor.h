#ifndef MILK_LUA_ACTOR_H
#define MILK_LUA_ACTOR_H

#include <string>

#include "math/Vector2.h"

namespace sol
{
    class state;
}

namespace milk
{
    class Actor;

    namespace lua
    {
        class LuaActor
        {
        public:
            static void bind(sol::state& luaState);

            static std::string name(Actor& actor);

            static Vector2 position(Actor& actor);

            static void move(Actor& actor, float x, float y);

            static void setAnimation(Actor& actor, const std::string& name);

            static void flipX(Actor& actor);

            static void flipY(Actor& actor);
        };
    }
}


#endif
