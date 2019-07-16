#ifndef _LUA_CONTROLLER_H_
#define _LUA_CONTROLLER_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_controller(lua_State* L);

#endif