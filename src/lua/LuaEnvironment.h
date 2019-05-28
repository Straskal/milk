#ifndef MILK_LUA_ENVIRONMENT_H
#define MILK_LUA_ENVIRONMENT_H

#include "data/int.h"
#include "main/milk.h"

struct lua_State;

namespace milk {
	class LuaEnvironment {
	public:
		virtual ~LuaEnvironment() = default;

		virtual void init(MilkState* milkState) = 0;
		virtual void free() = 0;

		virtual MilkStartupConfig getConfiguration(const std::string& configFile) = 0;

		virtual void addScript(U32 id, const std::string& scriptName) = 0;

		virtual void tick() = 0;
		virtual void postTick() = 0;
		virtual void render() = 0;
		virtual void postRender() = 0;
	};
}

#endif