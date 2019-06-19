#ifndef _LUA_KEYBOARD_H_
#define _LUA_KEYBOARD_H_

struct lua_State;

namespace milk {
	int luaopen_milk_keyboard(lua_State* L);
}

#endif