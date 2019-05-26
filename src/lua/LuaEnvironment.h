#ifndef MILK_LUA_ENVIRONMENT_H
#define MILK_LUA_ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <vector>

#include "data/int.h"
#include "main/milk.h"

struct lua_State;

namespace milk {
	class LuaEnvironment {
	public:
		void init();
		MilkStartupConfig getConfiguration(const std::string& configFile) const;

	private:
		lua_State* luaState_;
	};
}

#endif