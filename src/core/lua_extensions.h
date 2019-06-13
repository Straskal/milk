#ifndef _LUA_EXTENSIONS_H_
#define _LUA_EXTENSIONS_H_

#include <string>

struct lua_State;
struct luaL_Reg;

namespace milk {
	namespace luaM {
		void set_string_field(lua_State* L, const char* key, const char* value);
		std::string get_string_field(lua_State* L, const char* key);
		void set_int_field(lua_State* L, const char* key, int value);
		int get_int_field(lua_State* L, const char* key);
		int get_int_field(lua_State* L, int index, const char* key);
		bool get_bool_field(lua_State* L, const char* key);
		void invoke_method(lua_State* L, const char* key);
	}
}

#endif