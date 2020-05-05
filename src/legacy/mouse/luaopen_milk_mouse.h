#ifndef _LUA_MOUSE_H_
#define _LUA_MOUSE_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_mouse(lua_State* L);

#endif