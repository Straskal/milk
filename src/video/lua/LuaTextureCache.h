#ifndef MILK_LUA_TEXTURE_CACHE_H
#define MILK_LUA_TEXTURE_CACHE_H

struct lua_State;

namespace milk {
	class LuaTextureCache {
	public:
		static void set_texture_cache_submodule(lua_State* L);
	};
}

#endif