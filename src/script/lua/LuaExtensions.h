#ifndef MILK_LUA_EXTENSIONS_H
#define MILK_LUA_EXTENSIONS_H

#include <string>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace milk {
	namespace luax {
		std::string getStringField(lua_State* L, const std::string& key) {
			lua_getfield(L, -1, key.c_str());
			std::string result = std::string{ (const char*)lua_tostring(L, -1) };
			lua_pop(L, 1);
			return result;
		}
		
		int getIntegerField(lua_State* L, const std::string& key) {
			lua_getfield(L, -1, key.c_str());
			int result = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);
			return result;
		}

		bool getBooleanField(lua_State* L, const std::string& key) {
			lua_getfield(L, -1, key.c_str());
			bool result = (bool)lua_toboolean(L, -1);
			lua_pop(L, 1);
			return result;
		}
	}
}

#endif