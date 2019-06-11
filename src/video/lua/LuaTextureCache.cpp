#include "LuaTextureCache.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "video/Texture.h"
#include "core/Locator.h"

namespace {
	int reference(lua_State* L) {
		return 0;
	}

	int dereference(lua_State* L) {
		return 0;
	}
}

void milk::LuaTextureCache::set_texture_cache_submodule(lua_State* L) {

}
