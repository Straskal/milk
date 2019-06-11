#ifndef _LUA_TEXTURE_CACHE_H_
#define _LUA_TEXTURE_CACHE_H_

struct lua_State;

namespace milk {
	class LuaTextureCache {
	public:
		static void set_texture_cache_submodule(lua_State* L);
	};
}

#endif