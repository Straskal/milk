#ifndef _LUA_WINDOW_H_
#define _LUA_WINDOW_H_

struct lua_State;

namespace milk {
	class LuaWindow {
	public:
		static void pushWindowTable(lua_State* L);
	};
}

#endif