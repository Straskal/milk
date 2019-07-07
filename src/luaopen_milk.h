#ifndef _LUA_MILK_H_
#define _LUA_MILK_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk(lua_State* L);

#endif