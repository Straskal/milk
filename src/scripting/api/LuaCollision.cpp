#include "LuaCollision.h"

#include "physics/BoxCollider.h"
#include "physics/Collision.h"

namespace milk
{
    namespace lua
    {
        namespace collision_event
        {
            Actor& other(Collision& collision)
            {
                return collision.other.actor();
            }
        }
    }
}