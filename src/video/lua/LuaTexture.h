#ifndef _LUA_TEXTURE_H_
#define _LUA_TEXTURE_H_

struct lua_State;

namespace milk {
	int luaopen_texture(lua_State* L);
}

#endif