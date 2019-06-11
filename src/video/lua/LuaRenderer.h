#ifndef _LUA_RENDERER_H_
#define _LUA_RENDERER_H_

struct lua_State;

namespace milk {
	class LuaRenderer {
	public:
		static void set_renderer_submodule(lua_State* L);
	};
}

#endif