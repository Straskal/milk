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

		void tick();
		void postTick();
		void render();
		void postRender();

	private:
		lua_State* luaState_;
		std::unordered_map<U32, std::unordered_map<std::string, int>> scriptIdMap_;
		std::vector<int> newScripts_;
		std::vector<int> tickCallbacks_;
		std::vector<int> postTickCallbacks_;
		std::vector<int> renderCallbacks_;
	};
}

#endif