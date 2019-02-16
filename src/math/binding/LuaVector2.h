#ifndef MILK_LUAVECTOR2_H
#define MILK_LUAVECTOR2_H

namespace sol
{
    class state;
}

namespace milk
{
    namespace lua
    {
        class LuaVector2
        {
        public:
            static void bind(sol::state& luaState);
        };
    }
}

#endif
