#ifndef MILK_LUA_WINDOW_H
#define MILK_LUA_WINDOW_H

struct lua_State;

namespace milk {
	class LuaWindow {
	public:
		static void set_window_submodule(lua_State* L);
	};
}

#endif