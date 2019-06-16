#ifndef _LUA_RENDERER_H_
#define _LUA_RENDERER_H_

struct lua_State;

namespace milk {
	class LuaRenderer {
	public:
		static void pushRendererTable(lua_State* L);
	};
}

#endif