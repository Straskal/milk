#ifndef MILK_MOCK_SCRIPT_ENVIRONMENT
#define MILK_MOCK_SCRIPT_ENVIRONMENT

#include "gmock/gmock.h"
#include "script/ScriptEnvironment.h"

namespace milk {
	class MockScriptEnvironment : public ScriptEnvironment {
	public:
		MOCK_METHOD1(init, void(MilkState*));
		MOCK_METHOD0(free, void());
		MOCK_METHOD1(getConfiguration, MilkStartupConfig(const std::string&));
		MOCK_METHOD2(addScript, void(u32, const std::string&));
		MOCK_METHOD0(tick, void());
		MOCK_METHOD0(postTick, void());
		MOCK_METHOD0(render, void());
		MOCK_METHOD0(postRender, void());
	};
}

#endif