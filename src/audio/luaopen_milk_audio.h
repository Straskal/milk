#ifndef _LUA_AUDIO_H_
#define _LUA_AUDIO_H_

#include "milk_export.h"

struct lua_State;

extern "C" MILK_EXPORT int luaopen_milk_audio(lua_State* L);

#endif