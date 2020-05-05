#ifndef _LUA_WINDOW_H_
#define _LUA_WINDOW_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_window(lua_State* L);

#endif