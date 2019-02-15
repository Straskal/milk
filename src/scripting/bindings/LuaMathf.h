#ifndef MILK_LUAMATHF_H
#define MILK_LUAMATHF_H

namespace sol
{
    class state;
}

namespace milk
{
    namespace lua
    {
        class LuaMathf
        {
        public:
            static void bind(sol::state& luaState);
        };
    }
}

#endif
