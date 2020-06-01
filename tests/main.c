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
	INIT_TEST(milkCreate_InitializesFramebuffer),
	INIT_TEST(milkCreate_InitializesSpritesheet),
	INIT_TEST(milkCreate_InitializesFont),
	INIT_TEST(milkCreate_InitializesColorKey),
	INIT_TEST(milkCreate_InitializesClipRect),
	INIT_TEST(milkCreate_InitializesSamples),
	INIT_TEST(milkCreate_InitializesAudioQueue),
	INIT_TEST(milkCreate_InitializesAudioSettings),

	INIT_TEST(milkButton_WhenButtonDown_ReturnsTrue),
	INIT_TEST(milkButton_WhenButtonUp_ReturnsFalse),

	INIT_TEST(milkButtonPressed_WhenPressed_ReturnsTrue),
	INIT_TEST(milkButtonPressed_WhenHeld_ReturnsFalse),
	INIT_TEST(milkResetDrawState_ResetsColorKey),
	INIT_TEST(milkResetDrawState_ResetsClipRect),
	INIT_TEST(milkClipRect_ClampsClipRectToFramebufferSize),
	INIT_TEST(milkClipRect_SetsClipRect),
	INIT_TEST(milkPixelSet_WhenPixelWithinClipRect_SetsPixel),
	INIT_TEST(milkPixelSet_WhenPixelIsNotWithinClipRect_DoesNotSetPixel),

	INIT_TEST(milkSound_WhenIndexOutOfBounds_DoesNothing),
	INIT_TEST(milkSound_WhenSampleAtIndexNotLoaded_DoesNothing),
	INIT_TEST(milkSound_WhenMaxNumberOfConcurrentSoundsPlaying_DoesNothing),
	INIT_TEST(milkSound_QueuesNewSample),
	INIT_TEST(milkSound_WhenLoop_ExistingLoopingSampleIsRemovedFromQueueBeforeQueueingNewSample),
	INIT_TEST(milkSound_ClampsVolumeToMin),
	INIT_TEST(milkSound_ClampsVolumeToMax),
	INIT_TEST(milkVolume_ClampsToMinValue),
	INIT_TEST(milkVolume_ClampsToMaxValue),
	INIT_TEST(milkAudioQueueToStream_WhenQueueIsEmpty_DoesNotMixIntoStream),
	INIT_TEST(milkAudioQueueToStream_WhenQueueItemsAreNotFinished_MixesSamplesIntoStream),
	INIT_TEST(milkAudioQueueToStream_WhenSamplesIsFinishedAndLooping_ResetsSampleAndMixesIntoStream),
	INIT_TEST(milkAudioQueueToStream_WhenSamplesIsFinished_RemovesSampleFromQueue)
};

#define TEST_COUNT sizeof(_tests) / sizeof(Test)

int main(int argc, char *argv[])
{
	return runTests(_tests, TEST_COUNT);
}
