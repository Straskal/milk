#ifndef _LUA_MOUSE_H_
#define _LUA_MOUSE_H_

struct lua_State;

namespace milk {
	int luaopen_milk_mouse(lua_State* L);
}

#endif