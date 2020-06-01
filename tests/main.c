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
	REGISTER_TEST(milkAudioQueueToStream_WhenQueueIsEmpty_DoesNotMixIntoStream),
	REGISTER_TEST(milkAudioQueueToStream_WhenQueueItemsAreNotFinished_MixesSamplesIntoStream),
	REGISTER_TEST(milkAudioQueueToStream_WhenSamplesIsFinishedAndLooping_ResetsSampleAndMixesIntoStream),
	REGISTER_TEST(milkAudioQueueToStream_WhenSamplesIsFinished_RemovesSampleFromQueue)
	#undef REGISTER_TEST
};

#define TEST_COUNT sizeof(_tests) / sizeof(Test)

int main(int argc, char *argv[])
{
	return RUN_ALL_TESTS(_tests, TEST_COUNT);
}
