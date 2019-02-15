#ifndef MILK_LUA_API_H
#define MILK_LUA_API_H

namespace sol
{
    class state;
}

namespace milk
{
    // Registry for the Lua API.
    class LuaApi
    {
    public:
        static void init(sol::state& luaState);
    };
}

#endif
