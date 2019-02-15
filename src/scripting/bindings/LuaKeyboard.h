#ifndef MILK_LUAKEYBOARD_H
#define MILK_LUAKEYBOARD_H

namespace sol
{
    class state;
}

namespace milk
{
    namespace lua
    {
        // TODO: Eventually Keyboard will be a singleton, and then we can create binding passthrough methods.
        class LuaKeyboard
        {
        public:
            static void bind(sol::state& luaState);
        };
    }
}

#endif
