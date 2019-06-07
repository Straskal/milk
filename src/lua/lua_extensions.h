#ifndef MILK_LUA_EXTENSIONS_H
#define MILK_LUA_EXTENSIONS_H

#include <string>

struct lua_State;
struct luaL_Reg;

namespace milk {
	namespace lua {
		void set_string_field(lua_State* L, const char* key, const char* value);
		std::string get_string_field(lua_State* L, const char* key);
		void set_int_field(lua_State* L, const char* key, int value);
		int get_int_field(lua_State* L, const char* key);
		bool get_bool_field(lua_State* L, const char* key);
		void register_module(lua_State* L, const char* name, const luaL_Reg* funcs);
	}
}

#endif