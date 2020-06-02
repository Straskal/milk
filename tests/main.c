/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software andassociated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, andto permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice andthis permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "milk.h"
#include "milkassert.h"
#include "milktests.h"

static Test _tests[] =
{
	INIT_TEST(createMilk_InitializesFramebuffer),
	INIT_TEST(createMilk_InitializesSpritesheet),
	INIT_TEST(createMilk_InitializesFont),
	INIT_TEST(createMilk_InitializesColorKey),
	INIT_TEST(createMilk_InitializesClipRect),
	INIT_TEST(createMilk_InitializesSamples),
	INIT_TEST(createMilk_InitializesAudioQueue),
	INIT_TEST(createMilk_InitializesAudioSettings),

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
	INIT_TEST(playSound_WhenMaxNumberOfConcurrentSoundsPlaying_DoesNothing),
	INIT_TEST(playSound_QueuesNewSample),
	INIT_TEST(playSound_WhenLoop_ExistingLoopingSampleIsRemovedFromQueueBeforeQueueingNewSample),
	INIT_TEST(playSound_ClampsVolumeToMin),
	INIT_TEST(playSound_ClampsVolumeToMax),
	INIT_TEST(playSound_ClampsToMinValue),
	INIT_TEST(playSound_ClampsToMaxValue),
	INIT_TEST(mixSampleIntoStream_WhenQueueIsEmpty_DoesNotMixIntoStream),
	INIT_TEST(mixSampleIntoStream_WhenQueueItemsAreNotFinished_MixesSamplesIntoStream),
	INIT_TEST(mixSampleIntoStream_WhenSamplesIsFinishedAndLooping_ResetsSampleAndMixesIntoStream),
	INIT_TEST(mixSampleIntoStream_WhenSamplesIsFinished_RemovesSampleFromQueue)
};

#define TEST_COUNT sizeof(_tests) / sizeof(Test)

int main(int argc, char *argv[])
{
	return runTests(_tests, TEST_COUNT);
}
