#ifndef MILK_LUA_RENDERER_H
#define MILK_LUA_RENDERER_H

struct lua_State;

namespace milk {
	class LuaRenderer {
	public:
		static void set_renderer_submodule(lua_State* L);
	};
}

#endif