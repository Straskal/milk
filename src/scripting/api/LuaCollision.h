#ifndef MILK_LUA_COLLISION_EVENT_H
#define MILK_LUA_COLLISION_EVENT_H

namespace milk
{
    class Actor;
    class Collision;

    namespace lua
    {
        namespace collision_event
        {
            Actor& other(Collision& collision);
        }
    }
}

#endif
