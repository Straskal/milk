#ifndef _LUA_KEYBOARD_H_
#define _LUA_KEYBOARD_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_keyboard(lua_State* L);

#endif