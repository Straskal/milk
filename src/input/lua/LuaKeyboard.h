#ifndef _LUA_KEYBOARD_H_
#define _LUA_KEYBOARD_H_

struct lua_State;

namespace milk {
	class LuaKeyboard {
	public:
		static void pushKeyboardTable(lua_State* L);
	};
}

#endif