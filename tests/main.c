#include "milk.h"
#include "milkassert.h"
#include "milktests.h"

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
	REGISTER_TEST(milkClipRect_ClampsClipRectToFramebufferSize),
	REGISTER_TEST(milkClipRect_SetsClipRect),
	REGISTER_TEST(milkClear_SetsPixelsWithinClipRect),
	REGISTER_TEST(milkPixelSet_SetsPixelWithinClipRect),

	REGISTER_TEST(milkSound_WhenIndexOutOfBounds_DoesNothing),
	REGISTER_TEST(milkSound_WhenSampleAtIndexNotLoaded_DoesNothing),
	REGISTER_TEST(milkSound_WhenMaxNumberOfConcurrentSoundsPlaying_DoesNothing),
	REGISTER_TEST(milkSound_QueuesNewSample),
	REGISTER_TEST(milkSound_WhenLoop_ExistingLoopingSampleIsRemovedFromQueueBeforeQueueingNewSample),
	REGISTER_TEST(milkSound_ClampsVolumeToMin),
	REGISTER_TEST(milkSound_ClampsVolumeToMax),
	REGISTER_TEST(milkVolume_ClampsToMinValue),
	REGISTER_TEST(milkVolume_ClampsToMaxValue)
	#undef REGISTER_TEST
};

#define TEST_COUNT sizeof(_tests) / sizeof(Test)

int main(int argc, char *argv[])
{
	return RUN_ALL_TESTS(_tests, TEST_COUNT);
}
