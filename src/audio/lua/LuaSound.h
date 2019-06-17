#ifndef _LUA_SOUND_H_
#define _LUA_SOUND_H_

struct lua_State;

namespace milk {
	int luaopen_sound(lua_State* L);
}

#endif