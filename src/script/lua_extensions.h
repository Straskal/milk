#ifndef MILK_LUA_EXTENSIONS_H
#define MILK_LUA_EXTENSIONS_H

#include <string>

struct lua_State;

namespace milk {
	namespace lua {
		std::string get_string_field(lua_State* L, const char* key);
		int get_int_field(lua_State* L, const char* key);
		bool get_bool_field(lua_State* L, const char* key);
	}
}

#endif