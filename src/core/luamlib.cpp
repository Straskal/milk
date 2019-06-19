#include "luamlib.h"

#include "audio/luaopen_milk_audio.h"
#include "graphics/luaopen_milk_graphics.h"
#include "keyboard/luaopen_milk_keyboard.h"
#include "window/luaopen_milk_window.h"

static int correct_index(int index) {
	// When setting a field on a table:
	// If the index given if greater than zero, then we are working with an exact index.
	// If the index is less than zero, we're workin with indeces relative to the top of the stack.
	return index < 0 ? index - 1 : index;
}

void milk::luaM_preloadpackage(lua_State* L, const char* name, lua_CFunction openfunc) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, openfunc);
	lua_setfield(L, -2, name);
	// Pop the preload table and package table off of stack
	lua_pop(L, 2);
}

void milk::luaM_openlibs(lua_State* L) {
	luaM_preloadpackage(L, "milk.window", luaopen_milk_window);
	luaM_preloadpackage(L, "milk.keyboard", luaopen_milk_keyboard);
	luaM_preloadpackage(L, "milk.graphics", luaopen_milk_graphics);
	luaM_preloadpackage(L, "milk.audio", luaopen_milk_audio);
}

void milk::luaM_createmetatable(lua_State* L, const char* name, const luaL_Reg* funcs) {
	luaL_newmetatable(L, name);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, funcs, 0);
	lua_pop(L, 1); // Pop metatable off of stack
}

void milk::luaM_setstrfield(lua_State* L, int index, const char* key, const char* value) {
	lua_pushstring(L, value);
	lua_setfield(L, correct_index(index), key);
}

const char* milk::luaM_getstrfield(lua_State* L, int index, const char* key) {
	lua_getfield(L, index, key);
	const char* result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

void milk::luaM_setintfield(lua_State* L, int index, const char* key, int value) {
	lua_pushinteger(L, value);
	lua_setfield(L, correct_index(index), key);
}

int milk::luaM_getintfield(lua_State* L, int index, const char* key) {
	lua_getfield(L, index, key);
	int result = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

void milk::luaM_setnumfield(lua_State* L, int index, const char* key, double value) {
	lua_pushnumber(L, value);
	lua_setfield(L, correct_index(index), key);
}

double milk::luaM_getnumfield(lua_State* L, int index, const char* key) {
	lua_getfield(L, index, key);
	double result = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

bool milk::luaM_getboolfield(lua_State* L, int index, const char* key) {
	lua_getfield(L, index, key);
	bool result = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

void milk::luaM_setenumfield(lua_State* L, int index, const char* name, const luaM_Enum* e, size_t size) {
	lua_newtable(L);
	for (int i = 0; i < size; ++i) {
		luaM_setintfield(L, -1, e[i].name, e[i].key);
	}
	lua_setfield(L, correct_index(index), name);
}
