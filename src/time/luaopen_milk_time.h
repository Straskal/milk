#ifndef _LUA_TIME_H_
#define _LUA_TIME_H_

struct lua_State;

namespace milk
{
	int luaopen_milk_time(lua_State* L);
}

#endif