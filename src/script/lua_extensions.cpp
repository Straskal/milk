#include "lua_extensions.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

std::string milk::lua::get_string_field(lua_State* L, const char* key) {
	lua_getfield(L, -1, key);
	std::string result = std::string{ (const char*)lua_tostring(L, -1) };
	lua_pop(L, 1);
	return result;
}

int milk::lua::get_int_field(lua_State* L, const char* key) {
	lua_getfield(L, -1, key);
	int result = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

bool milk::lua::get_bool_field(lua_State* L, const char* key) {
	lua_getfield(L, -1, key);
	bool result = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}