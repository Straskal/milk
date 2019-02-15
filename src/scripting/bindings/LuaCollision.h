#ifndef MILK_LUA_COLLISION_EVENT_H
#define MILK_LUA_COLLISION_EVENT_H

namespace sol
{
    class state;
}

namespace milk
{
    class Actor;
    class Collision;

    namespace lua
    {
        class LuaCollision
        {
        public:
            static void bind(sol::state& luaState);

            static Actor& other(Collision& collision);;
        };
    }
}

#endif
