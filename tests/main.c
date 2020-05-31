#include "milk.h"
#include "milkassert.h"
#include "milktests.h"

static Test _tests[] =
{
	#define REGISTER_TEST(func) { #func, func }
	REGISTER_TEST(milkCreate_InitializesFramebuffer),
	REGISTER_TEST(milkCreate_InitializesSpritesheet),
	REGISTER_TEST(milkCreate_InitializesFont),
	REGISTER_TEST(milkCreate_InitializesColorKey),
	REGISTER_TEST(milkCreate_InitializesClipRect),
	REGISTER_TEST(milkCreate_InitializesSamples),
	REGISTER_TEST(milkCreate_InitializesAudioQueue),
	REGISTER_TEST(milkCreate_InitializesAudioSettings),

	REGISTER_TEST(milkButton_WhenButtonDown_ReturnsTrue),
	REGISTER_TEST(milkButton_WhenButtonUp_ReturnsFalse),

	REGISTER_TEST(milkButtonPressed_WhenPressed_ReturnsTrue),
	REGISTER_TEST(milkButtonPressed_WhenHeld_ReturnsFalse),
	REGISTER_TEST(milkResetDrawState_ResetsColorKey),
	REGISTER_TEST(milkResetDrawState_ResetsClipRect),
	REGISTER_TEST(milkClipRect_ClampsClipRectToFramebufferSize),
	REGISTER_TEST(milkClipRect_SetsClipRect),
	REGISTER_TEST(milkClear_SetsPixelsWithinClipRect),
	REGISTER_TEST(milkPixelSet_WhenPixelWithinClipRect_SetsPixel),
	REGISTER_TEST(milkPixelSet_WhenPixelIsNotWithinClipRect_DoesNotSetPixel),

	REGISTER_TEST(milkSound_WhenIndexOutOfBounds_DoesNothing),
	REGISTER_TEST(milkSound_WhenSampleAtIndexNotLoaded_DoesNothing),
	REGISTER_TEST(milkSound_WhenMaxNumberOfConcurrentSoundsPlaying_DoesNothing),
	REGISTER_TEST(milkSound_QueuesNewSample),
	REGISTER_TEST(milkSound_WhenLoop_ExistingLoopingSampleIsRemovedFromQueueBeforeQueueingNewSample),
	REGISTER_TEST(milkSound_ClampsVolumeToMin),
	REGISTER_TEST(milkSound_ClampsVolumeToMax),
	REGISTER_TEST(milkVolume_ClampsToMinValue),
	REGISTER_TEST(milkVolume_ClampsToMaxValue),
	REGISTER_TEST(milkMixCallback_WhenQueueIsEmpty_DoesNotMixStream),
	REGISTER_TEST(milkMixCallback_WhenSamplesAreNotFinished_MixesSamplesIntoStream),
	REGISTER_TEST(milkMixCallback_WhenSamplesIsFinishedAndLooping_ResetsSampleAndMixesIntoStream),
	REGISTER_TEST(milkMixCallback_WhenSamplesIsFinished_RemovedSampleFromQueue)
	#undef REGISTER_TEST
};

#define TEST_COUNT sizeof(_tests) / sizeof(Test)

int main(int argc, char *argv[])
{
	return RUN_ALL_TESTS(_tests, TEST_COUNT);
}
