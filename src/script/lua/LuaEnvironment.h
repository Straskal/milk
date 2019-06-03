#ifndef MILK_LUA_ENVIRONMENT_H
#define MILK_LUA_ENVIRONMENT_H

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "data/int.h"
#include "script/ScriptEnvironment.h"

struct lua_State;

namespace milk {
	namespace lua {
		class LuaEnvironment : public ScriptEnvironment {
		public:
			void init() override;
			void free() override;

			MilkStartupConfig getConfiguration(const std::string& configFile) override;

			void tick() override;
			void postTick() override;
			void render() override;
			void postRender() override;

		private:
			lua_State* m_L;
			std::unordered_map<u32, int> m_scripts;
			std::vector<int> m_newScripts;
			std::vector<int> m_tickCallbacks;
			std::vector<int> m_postTickCallbacks;
			std::vector<int> m_renderCallbacks;
			std::vector<int> m_postRenderCallbacks;
		};
	}
}

#endif