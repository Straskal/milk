#include "LuaMathf.h"

#include "sol/sol.hpp"

#include "math/Mathf.h"

void milk::lua::LuaMathf::bind(sol::state& luaState)
{
    luaState.new_usertype<Mathf>("Mathf",
                                 "clamp", &Mathf::clamp);
}
