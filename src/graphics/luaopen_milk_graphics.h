#ifndef _LUA_GRAPHICS_H_
#define _LUA_GRAPHICS_H_

struct lua_State;

namespace milk
{
	int luaopen_milk_graphics(lua_State* L);
}

#endif