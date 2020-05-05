#include "luamlib.h"

#include "audio/luaopen_milk_audio.h"
#include "graphics/luaopen_milk_graphics.h"
#include "mouse/luaopen_milk_mouse.h"
#include "keyboard/luaopen_milk_keyboard.h"
#include "time/luaopen_milk_time.h"
#include "window/luaopen_milk_window.h"

static int correct_index(int index)
{
	/*
		When setting a field on a table:
		If the index given if greater than zero, then we are working with an exact index.
		If the index is less than zero, we're workin with indeces relative to the top of the stack.
	*/
	return index < 0 ? index - 1 : index;
}

static void luaM_openlib(lua_State* L, const char* name, lua_CFunction openfunc)
{
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, openfunc);
	lua_setfield(L, -2, name);
	lua_pop(L, 1); /* Pop the preload table off of the stack */
	lua_getfield(L, -1, "loaded");
	lua_pushcfunction(L, openfunc);
	lua_pcall(L, 0, 1, 0);
	lua_setfield(L, -1, name);
	lua_pop(L, 2); /* Pop the loaded table and package table off of stack */
}

void milk::luaM_openlibs(lua_State* L)
{
	luaM_openlib(L, "milk.time", luaopen_milk_time);
	luaM_openlib(L, "milk.window", luaopen_milk_window);
	luaM_openlib(L, "milk.mouse", luaopen_milk_mouse);
	luaM_openlib(L, "milk.keyboard", luaopen_milk_keyboard);
	luaM_openlib(L, "milk.graphics", luaopen_milk_graphics);
	luaM_openlib(L, "milk.audio", luaopen_milk_audio);
}

void milk::luaM_createmetatable(lua_State* L, const char* name, const luaL_Reg* funcs)
{
	luaL_newmetatable(L, name);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, funcs, 0);
	lua_pop(L, 1); /* Pop metatable off of stack */
}

void milk::luaM_setintfield(lua_State* L, int index, const char* key, int value)
{
	lua_pushinteger(L, value);
	lua_setfield(L, correct_index(index), key);
}

void milk::luaM_setenumfield(lua_State* L, int index, const char* name, const luaM_Enum* e, size_t size)
{
	lua_newtable(L);
	unsigned int sz = size / sizeof(luaM_Enum);
	for (unsigned int i = 0; i < sz; ++i)
		luaM_setintfield(L, -1, e[i].name, e[i].key);
	lua_setfield(L, correct_index(index), name);
}
