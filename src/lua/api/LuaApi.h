#ifndef MILK_LUA_API_H
#define MILK_LUA_API_H

struct lua_State;

namespace milk {
	class LuaApi {
	public:
		static void bind(lua_State* L);
	};
}

#endif