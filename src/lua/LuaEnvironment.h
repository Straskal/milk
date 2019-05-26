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
		void doFile(const std::string& file);
		std::string getStringField(const std::string& key);
		int getIntegerField(const std::string& key);
		bool getBooleanField(const std::string& key);

	private:
		lua_State* luaState_;
	};

	namespace luaExtensions {
		MilkStartupConfig getConfiguration(LuaEnvironment& env, const std::string& configFile);
	}
}

#endif