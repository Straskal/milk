#ifndef _LUA_WINDOW_H_
#define _LUA_WINDOW_H_

struct lua_State;

namespace milk {
	int luaopen_window(lua_State* L);
}

#endif