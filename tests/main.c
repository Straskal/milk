#include "milk.h"
#include "milkassert.h"
#include "milktests.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static Test _tests[] =
{
	#define REGISTER_TEST(func) { #func, func }
	REGISTER_TEST(milkCreate_InitializesMilk),
	REGISTER_TEST(milkButton_WhenButtonDown_ReturnsTrue),
	REGISTER_TEST(milkButton_WhenButtonUp_ReturnsFalse),
	REGISTER_TEST(milkButtonPressed_WhenPressed_ReturnsTrue),
	REGISTER_TEST(milkButtonPressed_WhenHeld_ReturnsFalse),
	REGISTER_TEST(milkResetDrawState_ResetsColorKey),
	REGISTER_TEST(milkResetDrawState_ResetsClipRect),
	REGISTER_TEST(milkClipRect_LimitsClipRectToFramebufferSize),
	REGISTER_TEST(milkClipRect_SetsClipRect),
	REGISTER_TEST(milkClear_SetsPixelsWithinClipRect),
	REGISTER_TEST(milkPixelSet_SetsPixelWithinClipRect)
	#undef TEST
};

#define TEST_COUNT sizeof(_tests) / sizeof(Test)

int main(int argc, char *argv[])
{
	RUN_ALL_TESTS(_tests, TEST_COUNT);
}
