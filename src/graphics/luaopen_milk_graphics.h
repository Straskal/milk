#ifndef _LUA_GRAPHICS_H_
#define _LUA_GRAPHICS_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_graphics(lua_State* L);

#endif