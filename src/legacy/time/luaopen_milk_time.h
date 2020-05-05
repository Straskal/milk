#ifndef _LUA_TIME_H_
#define _LUA_TIME_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_time(lua_State* L);

#endif