#include "audio_tests.h"
#include "common.h"
#include "milktests.h"

static Test _tests[] =
{
	// Input
	INIT_TEST(isButtonDown_WhenButtonDown_ReturnsTrue),
	INIT_TEST(isButtonDown_WhenButtonUp_ReturnsFalse),
	INIT_TEST(isButtonPressed_WhenPressed_ReturnsTrue),
	INIT_TEST(isButtonPressed_WhenHeld_ReturnsFalse),

	// Audio
	INIT_TEST(initAudio_InitializesSounds),
	INIT_TEST(initAudio_InitializesSoundSlots),
	INIT_TEST(initAudio_InitializesStreams),
	INIT_TEST(initAudio_InitializesStreamSlot),
	INIT_TEST(initAudio_InitializesMasterVolume),

	INIT_TEST(playSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(playSound_WhenSampleAtIndexNotLoaded_DoesNothing),
	INIT_TEST(playSound_WhenSampleLengthIsZero_DoesNothing),
	INIT_TEST(playSound_SetsSlot),
	INIT_TEST(playSound_ClampsVolumeToMin),
	INIT_TEST(playSound_ClampsVolumeToMax),
	INIT_TEST(setMasterVolume_ClampsToMinValue),
	INIT_TEST(setMasterVolume_ClampsToMaxValue),
	INIT_TEST(stopSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(stopSound_StopsSound),
	INIT_TEST(pauseSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(pauseSound_PausesSound),
	INIT_TEST(resumeSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(resumeSound_resumesSound),

	// Video
	INIT_TEST(initVideo_InitializesFramebuffer),
	INIT_TEST(initVideo_InitializesSpritesheet),
	INIT_TEST(initVideo_InitializesFont),
	INIT_TEST(initVideo_InitializesColorKey),
	INIT_TEST(initVideo_InitializesClipRect),
	INIT_TEST(resetDrawState_ResetsColorKey),
	INIT_TEST(resetDrawState_ResetsClipRect),
	INIT_TEST(setClippingRect_ClampsClipRectToFramebufferSize),
	INIT_TEST(setClippingRect_SetsClipRect),
	INIT_TEST(blitPixel_WhenPixelWithinClipRect_SetsPixel),
	INIT_TEST(blitPixel_WhenPixelIsNotWithinClipRect_DoesNotSetPixel),
};

#define TEST_COUNT (sizeof(_tests) / sizeof(Test))

int main(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	return runTests(_tests, TEST_COUNT);
}
