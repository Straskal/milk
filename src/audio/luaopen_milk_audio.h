#ifndef _LUA_AUDIO_H_
#define _LUA_AUDIO_H_

struct lua_State;

namespace milk
{
	int luaopen_milk_audio(lua_State* L);
}

#endif