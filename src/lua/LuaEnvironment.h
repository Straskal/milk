#ifndef MILK_LUA_ENVIRONMENT_H
#define MILK_LUA_ENVIRONMENT_H

#include <queue>
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
		void free();

		MilkStartupConfig getConfiguration(const std::string& configFile);

		void addScript(U32 id, const std::string& scriptName);
		void removeScript(U32 id, const std::string& scriptName);
		int getScript(U32 id, const std::string& scriptName);

		void tick();
		void postTick();
		void render();

	private:
		lua_State* luaState_;
		std::unordered_map<U32, std::unordered_map<std::string, int>> scriptIdMap_;
		std::vector<int> newScripts_;
		std::vector<int> tickCallbacks_;
		std::vector<int> postTickCallbacks_;
		std::vector<int> renderCallbacks_;
	};

	namespace lua {
		std::string getStringField(lua_State* L, const std::string& key);
		int getIntegerField(lua_State* L, const std::string& key);
		bool getBooleanField(lua_State* L, const std::string& key);
	}
}

#endif