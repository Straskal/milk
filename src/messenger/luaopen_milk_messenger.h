#ifndef _LUA_MESSENGER_H_
#define _LUA_MESSENGER_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_messenger(lua_State* L);

#endif