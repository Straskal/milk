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

int main(int argc, char *argv[])
{
	const size_t testCount = sizeof(_tests) / sizeof(Test);
	int passedCount = 0;

	printf("Running tests\n\n\n");

	for (size_t i = 0; i < testCount; i++)
	{
		_tests[i].failedAssertCount = 0;
		gCurrentTest = &_tests[i];
		_tests[i].testFunc();
	}

	for (size_t i = 0; i < testCount; i++)
		if (_tests[i].failedAssertCount == 0)
			passedCount++;

	printf("Passed %d/%d\n", passedCount, testCount);
	printf("=======================================\n\n");

	for (size_t i = 0; i < testCount; i++)
		if (_tests[i].failedAssertCount == 0)
			printf("	- %s\n", _tests[i].name);

	printf("\nFailed %d/%d\n", testCount - passedCount, testCount);
	printf("=======================================\n\n");

	for (size_t i = 0; i < testCount; i++)
		if (_tests[i].failedAssertCount > 0)
		{
			printf("	- %s\n", _tests[i].name);

			for (int j = 0; j < _tests[i].failedAssertCount; j++)
				printf("		%s\n", _tests[i].failedAsserts[j]);
		}
}
