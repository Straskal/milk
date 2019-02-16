#include "LuaCollision.h"

#include "sol/sol.hpp"

#include "physics/BoxCollider.h"
#include "physics/Collision.h"

#include "scene/Actor.h"

namespace milk
{
    namespace lua
    {
        void LuaCollision::bind(sol::state& luaState)
        {
            luaState.new_usertype<Collision>("Collision",
                                             "other", sol::readonly_property(&LuaCollision::other));
        }

        Actor& LuaCollision::other(Collision& collision)
        {
            return collision.other.actor();
        }
    }
}