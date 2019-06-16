#include "lua_extensions.h"

#include <iostream>
#include <cmath>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

void milk::luaM::set_string_field(lua_State* L, const char* key, const char* value) {
	lua_pushstring(L, value);
	lua_setfield(L, -2, key);
}

std::string milk::luaM::get_string_field(lua_State* L, const char* key) {
	lua_getfield(L, -1, key);
	std::string result = std::string{ (const char*)lua_tostring(L, -1) };
	lua_pop(L, 1);
	return result;
}

void milk::luaM::set_int_field(lua_State* L, const char* key, int value) {
	lua_pushinteger(L, value);
	lua_setfield(L, -2, key);
}

int milk::luaM::get_int_field(lua_State* L, const char* key) {
	lua_getfield(L, -1, key);
	int result = std::round(lua_tonumber(L, -1));
	lua_pop(L, 1);
	return result;
}

int milk::luaM::get_int_field(lua_State* L, int index, const char* key) {
	lua_getfield(L, index, key);
	int result = std::round(lua_tonumber(L, -1));
	lua_pop(L, 1);
	return result;
}

bool milk::luaM::get_bool_field(lua_State* L, const char* key) {
	lua_getfield(L, -1, key);
	bool result = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}
