#include "LuaMathf.h"

#include "scripting/sol/sol.hpp"

#include "math/Mathf.h"

void milk::lua::LuaMathf::bind(sol::state& luaState)
{
	luaState.new_usertype<Mathf>("Mathf",
		"clamp", &Mathf::clamp);
}
