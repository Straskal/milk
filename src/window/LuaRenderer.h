#ifndef MILK_LUA_RENDERER_H
#define MILK_LUA_RENDERER_H

struct lua_State;

namespace milk {
	class LuaRenderer {
	public:
		static void bind(lua_State* L);
	};
}

#endif