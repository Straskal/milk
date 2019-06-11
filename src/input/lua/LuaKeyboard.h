#ifndef MILK_LUA_KEYBOARD_H
#define MILK_LUA_KEYBOARD_H

struct lua_State;

namespace milk {
	class LuaKeyboard {
	public:
		static void set_keyboard_submodule(lua_State* L);
	};
}

#endif