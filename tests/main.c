#include "milkassert.h"
#include "milktests.h"

static Test _tests[] =
{
	INIT_TEST(initVideo_InitializesFramebuffer),
	INIT_TEST(initVideo_InitializesSpritesheet),
	INIT_TEST(initVideo_InitializesFont),
	INIT_TEST(initVideo_InitializesColorKey),
	INIT_TEST(initVideo_InitializesClipRect),
	INIT_TEST(initAudio_InitializesSamples),
	INIT_TEST(initAudio_InitializesAudioSlots),
	INIT_TEST(initAudio_InitializesAudioSettings),

	INIT_TEST(isButtonDown_WhenButtonDown_ReturnsTrue),
	INIT_TEST(isButtonDown_WhenButtonUp_ReturnsFalse),
	INIT_TEST(isButtonPressed_WhenPressed_ReturnsTrue),
	INIT_TEST(isButtonPressed_WhenHeld_ReturnsFalse),

	INIT_TEST(resetDrawState_ResetsColorKey),
	INIT_TEST(resetDrawState_ResetsClipRect),
	INIT_TEST(setClippingRect_ClampsClipRectToFramebufferSize),
	INIT_TEST(setClippingRect_SetsClipRect),
	INIT_TEST(blitPixel_WhenPixelWithinClipRect_SetsPixel),
	INIT_TEST(blitPixel_WhenPixelIsNotWithinClipRect_DoesNotSetPixel),

	INIT_TEST(playSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(playSound_WhenSampleAtIndexNotLoaded_DoesNothing),
	INIT_TEST(playSound_WhenSampleLengthIsZero_DoesNothing),
	INIT_TEST(playSound_SetsSlot),
	INIT_TEST(playSound_ClampsVolumeToMin),
	INIT_TEST(playSound_ClampsVolumeToMax),
	INIT_TEST(setMasterVolume_ClampsToMinValue),
	INIT_TEST(setMasterVolume_ClampsToMaxValue),
	INIT_TEST(stopSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(stopSound_WhenIndexIsNegativeOne_StopsAllPlayingSounds),
	INIT_TEST(stopSound_StopsSound),
	INIT_TEST(pauseSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(pauseSound_WhenIndexIsNegativeOne_PausesAllPlayingSounds),
	INIT_TEST(pauseSound_PausesSound),
	INIT_TEST(resumeSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(resumeSound_WhenIndexIsNegativeOne_PausesAllPausedSounds),
	INIT_TEST(resumeSound_resumesSound)
};

#define TEST_COUNT (sizeof(_tests) / sizeof(Test))

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	return runTests(_tests, TEST_COUNT);
}
