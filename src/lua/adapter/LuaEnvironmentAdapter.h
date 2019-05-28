#ifndef MILK_LUA_ENVIRONMENT_ADAPTER_H
#define MILK_LUA_ENVIRONMENT_ADAPTER_H

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "data/int.h"
#include "lua/LuaEnvironment.h"

struct lua_State;

namespace milk {
	namespace adapter {
		class LuaEnvironmentAdapter : public LuaEnvironment {
		public:
			void init(MilkState* milkState) override;
			void free() override;

			MilkStartupConfig getConfiguration(const std::string& configFile) override;

			void addScript(U32 id, const std::string& scriptName) override;

			void tick() override;
			void postTick() override;
			void render() override;
			void postRender() override;

		private:
			lua_State* luaState_;
			std::unordered_map<U32, std::unordered_map<std::string, int>> scriptIdMap_;
			std::vector<int> newScripts_;
			std::vector<int> tickCallbacks_;
			std::vector<int> postTickCallbacks_;
			std::vector<int> renderCallbacks_;
		};
	}
}

#endif