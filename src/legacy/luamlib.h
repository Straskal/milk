#ifndef _LUA_MILK_LIB_H_
#define _LUA_MILK_LIB_H_

#include <string>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

namespace milk
{
	struct luaM_Enum
	{
		const char* name;
		int key;
	};

	void luaM_openlibs(lua_State* L);
	void luaM_createmetatable(lua_State* L, const char* name, const luaL_Reg* funcs);
	void luaM_setintfield(lua_State* L, int index, const char* key, int value);
	void luaM_setenumfield(lua_State* L, int index, const char* name, const luaM_Enum* e, size_t size);
}

#endif