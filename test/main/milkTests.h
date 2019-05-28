#ifndef MILK_TESTS_H
#define MILK_TESTS_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "script/MockScriptEnvironment.h"
#include "window/MockWindow.h"
#include "window/MockRenderer.h"

using ::testing::Return;
using ::testing::Sequence;
using ::testing::_;

namespace milk {
	TEST(MilkTests, Init_WindowFailure_ReturnsFalse) {
		MilkState m;

		MockScriptEnvironment* env = new MockScriptEnvironment();
		MilkStartupConfig config;

		EXPECT_CALL(*env, init(&m));
		EXPECT_CALL(*env, getConfiguration(_))
			.WillOnce(Return(config));

		MockWindow* win = new MockWindow();

		EXPECT_CALL(*win, init(_, _, _, _))
			.WillOnce(Return(false));

		m.scriptenv = env;
		m.window = win;

		ASSERT_FALSE(milk::state::init(m));

		delete win;
		delete env;
	}

	TEST(MilkTests, Init_RendererFailure_ReturnsFalse) {
		MilkState m;
		MockScriptEnvironment* env = new MockScriptEnvironment();
		MilkStartupConfig config;
		EXPECT_CALL(*env, init(&m));
		EXPECT_CALL(*env, getConfiguration(_))
			.WillOnce(Return(config));

		MockWindow* win = new MockWindow();
		EXPECT_CALL(*win, init(_, _, _, _))
			.WillOnce(Return(true));

		MockRenderer* ren = new MockRenderer();
		EXPECT_CALL(*ren, init(_, _, _))
			.WillOnce(Return(false));

		m.scriptenv = env;
		m.window = win;
		m.renderer = ren;

		ASSERT_FALSE(milk::state::init(m));

		delete ren;
		delete win;
		delete env;
	}

	TEST(MilkTests, Init_SetsMilkStateRunning) {
		MilkState m;
		MockScriptEnvironment* env = new MockScriptEnvironment();
		MilkStartupConfig config;
		EXPECT_CALL(*env, init(&m));
		EXPECT_CALL(*env, getConfiguration(_))
			.WillOnce(Return(config));

		MockWindow* win = new MockWindow();
		EXPECT_CALL(*win, init(_, _, _, _))
			.WillOnce(Return(true));

		MockRenderer* ren = new MockRenderer();
		EXPECT_CALL(*ren, init(_, _, _))
			.WillOnce(Return(true));

		m.scriptenv = env;
		m.window = win;
		m.renderer = ren;

		ASSERT_TRUE(milk::state::init(m));
		ASSERT_TRUE(m.running);

		delete ren;
		delete win;
		delete env;
	}

	TEST(MilkTests, Tick) {
		MilkState m;
		MockScriptEnvironment* env = new MockScriptEnvironment();
		EXPECT_CALL(*env, tick());

		m.scriptenv = env;
		milk::state::tick(m);
		delete env;
	}

	TEST(MilkTests, PostTick) {
		MilkState m;
		MockScriptEnvironment* env = new MockScriptEnvironment();
		EXPECT_CALL(*env, postTick());

		m.scriptenv = env;
		milk::state::postTick(m);
		delete env;
	}

	TEST(MilkTests, Render) {
		MilkState m;
		MockScriptEnvironment* env = new MockScriptEnvironment();
		MockRenderer* ren = new MockRenderer();

		Sequence seq;
		EXPECT_CALL(*ren, clear(_));
		EXPECT_CALL(*env, render());
		EXPECT_CALL(*ren, present());

		m.scriptenv = env;
		m.renderer = ren;
		milk::state::render(m);
		delete env;
		delete ren;
	}

	TEST(MilkTests, PostRender) {
		MilkState m;
		MockScriptEnvironment* env = new MockScriptEnvironment();
		EXPECT_CALL(*env, postRender());

		m.scriptenv = env;
		milk::state::postRender(m);
		delete env;
	}
}

#endif