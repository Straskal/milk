#ifndef MILK_SCRIPT_ENVIRONMENT_H
#define MILK_SCRIPT_ENVIRONMENT_H

#include <string>

#include "data/int.h"
#include "main/milk.h"

struct lua_State;

namespace milk {
	class ScriptEnvironment {
	public:
		virtual ~ScriptEnvironment() = default;

		virtual void init() = 0;
		virtual void free() = 0;

		virtual MilkStartupConfig getConfiguration(const std::string& configFile) = 0;

		virtual void tick() = 0;
		virtual void postTick() = 0;
		virtual void render() = 0;
		virtual void postRender() = 0;
	};
}

#endif