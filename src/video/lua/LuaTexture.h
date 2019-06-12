#ifndef _LUA_TEXTURE_H_
#define _LUA_TEXTURE_H_

struct lua_State;

namespace milk {
	class LuaTexture {
	public:
		static void set_texture_submodule(lua_State* L);
	};
}

#endif