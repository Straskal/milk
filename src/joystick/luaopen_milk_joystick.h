#ifndef _LUA_JOYSTICK_H_
#define _LUA_JOYSTICK_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_joystick(lua_State* L);

#endif