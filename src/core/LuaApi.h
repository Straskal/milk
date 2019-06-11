#ifndef _LUA_API_H_
#define _LUA_API_H_

struct lua_State;

namespace milk {
	class LuaApi {
	public:
		static void open(lua_State* L);
	};
}

#endif