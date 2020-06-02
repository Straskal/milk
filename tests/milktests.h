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

#ifndef __MILK_TESTS_H__
#define __MILK_TESTS_H__

#include "milk.h"
#include "milkassert.h"

#include <string.h>

#define FRAMEBUFFER_POS(x, y)			((MILK_FRAMEBUF_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y)	(clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

/*
 *******************************************************************************
 * Initialization and shutdown
 *******************************************************************************
 */

TEST_CASE(milkCreate_InitializesFramebuffer)
{
	SETUP(milk);

	for (int i = 0; i < MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT; i++)
		ASSERT_EQ(0x00, milk->video.framebuffer[i]);

	TEARDOWN(milk);
}

TEST_CASE(milkCreate_InitializesSpritesheet)
{
	SETUP(milk);

	for (int i = 0; i < MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SQRSIZE; i++)
		ASSERT_EQ(0x00, milk->video.spritesheet[i]);

	TEARDOWN(milk);
}

TEST_CASE(milkCreate_InitializesFont)
{
	SETUP(milk);

	for (int i = 0; i < MILK_FONT_WIDTH * MILK_FONT_HEIGHT; i++)
		ASSERT_EQ(0x00, milk->video.font[i]);

	TEARDOWN(milk);
}

TEST_CASE(milkCreate_InitializesColorKey)
{
	SETUP(milk);
	ASSERT_EQ(0x00, milk->video.colorKey);
	TEARDOWN(milk);
}

TEST_CASE(milkCreate_InitializesClipRect)
{
	SETUP(milk);
	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(MILK_FRAMEBUF_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(MILK_FRAMEBUF_WIDTH, milk->video.clipRect.right);
	TEARDOWN(milk);
}

TEST_CASE(milkCreate_InitializesSamples)
{
	SETUP(milk);

	for (int i = 0; i < MILK_AUDIO_MAX_SOUNDS; i++)
	{
		ASSERT_NULL(milk->audio.samples[i].buffer);
		ASSERT_EQ(0, milk->audio.samples[i].length);
	}

	TEARDOWN(milk);
}

TEST_CASE(milkCreate_InitializesAudioQueue)
{
	//SETUP(milk);

	//for (int i = 0; i < MILK_AUDIO_QUEUE_MAX; i++)
	//{
	//	ASSERT_NULL(milk->audio.queueItems[i].sampleData);
	//	ASSERT_EQ(0, milk->audio.queueItems[i].remainingLength);
	//	ASSERT_NULL(milk->audio.queueItems[i].position);
	//	ASSERT_EQ(0, milk->audio.queueItems[i].volume);
	//	ASSERT_FALSE(milk->audio.queueItems[i].isLooping);
	//	ASSERT_TRUE(milk->audio.queueItems[i].isFree);
	//}

	//ASSERT_NNULL(milk->audio.queue);
	//TEARDOWN(milk);
}

TEST_CASE(milkCreate_InitializesAudioSettings)
{
	SETUP(milk);
	ASSERT_EQ(MILK_AUDIO_MAX_VOLUME, milk->audio.masterVolume);
	ASSERT_EQ(0, milk->audio.frequency);
	ASSERT_EQ(0, milk->audio.channels);
	TEARDOWN(milk);
}

/*
 *******************************************************************************
 * Input
 *******************************************************************************
 */

TEST_CASE(milkButton_WhenButtonDown_ReturnsTrue)
{
	SETUP(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;

	bool isDown = ACT(milkButton(&milk->input, BTN_DOWN));

	ASSERT_TRUE(isDown);
	TEARDOWN(milk);
}

TEST_CASE(milkButton_WhenButtonUp_ReturnsFalse)
{
	SETUP(milk);
	milk->input.gamepad.buttonState = 0;

	bool isDown = ACT(milkButton(&milk->input, BTN_DOWN));

	ASSERT_FALSE(isDown);
	TEARDOWN(milk);
}

TEST_CASE(milkButtonPressed_WhenPressed_ReturnsTrue)
{
	SETUP(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;
	milk->input.gamepad.previousButtonState = 0;

	bool isPressed = ACT(milkButtonPressed(&milk->input, BTN_DOWN));

	ASSERT_TRUE(isPressed);
	TEARDOWN(milk);
}

TEST_CASE(milkButtonPressed_WhenHeld_ReturnsFalse)
{
	SETUP(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;
	milk->input.gamepad.previousButtonState |= BTN_DOWN;

	bool isPressed = ACT(milkButtonPressed(&milk->input, BTN_DOWN));

	ASSERT_FALSE(isPressed);
	TEARDOWN(milk);
}

/*
 *******************************************************************************
 * Video
 *******************************************************************************
 */

TEST_CASE(milkResetDrawState_ResetsColorKey)
{
	SETUP(milk);
	milk->video.colorKey = 57;

	ACT(milkResetDrawState(&milk->video));

	ASSERT_EQ(0x00, milk->video.colorKey);
	TEARDOWN(milk);
}

TEST_CASE(milkResetDrawState_ResetsClipRect)
{
	SETUP(milk);

	milk->video.clipRect.top = 10;
	milk->video.clipRect.bottom = 10;
	milk->video.clipRect.left = 10;
	milk->video.clipRect.right = 10;

	ACT(milkResetDrawState(&milk->video));

	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(MILK_FRAMEBUF_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(MILK_FRAMEBUF_WIDTH, milk->video.clipRect.right);
	TEARDOWN(milk);
}

TEST_CASE(milkClipRect_ClampsClipRectToFramebufferSize)
{
	SETUP(milk);

	ACT(milkClipRect(&milk->video, -10, -10, 500, 500));

	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(MILK_FRAMEBUF_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(MILK_FRAMEBUF_WIDTH, milk->video.clipRect.right);
	TEARDOWN(milk);
}

TEST_CASE(milkClipRect_SetsClipRect)
{
	SETUP(milk);

	ACT(milkClipRect(&milk->video, 10, 20, 200, 100));

	ASSERT_EQ(20, milk->video.clipRect.top);
	ASSERT_EQ(120, milk->video.clipRect.bottom);
	ASSERT_EQ(10, milk->video.clipRect.left);
	ASSERT_EQ(210, milk->video.clipRect.right);
	TEARDOWN(milk);
}

TEST_CASE(milkPixelSet_WhenPixelWithinClipRect_SetsPixel)
{
	SETUP(milk);
	milkClear(&milk->video, 0x00);
	ACT(milkPixelSet(&milk->video, 15, 30, 0x00ff00));
	ASSERT_EQ(0x00ff00, milk->video.framebuffer[FRAMEBUFFER_POS(15, 30)]);
	TEARDOWN(milk);
}

TEST_CASE(milkPixelSet_WhenPixelIsNotWithinClipRect_DoesNotSetPixel)
{
	SETUP(milk);
	milkClear(&milk->video, 0x00);
	milkClipRect(&milk->video, 10, 20, 200, 100);
	ACT(milkPixelSet(&milk->video, 1, 1, 0xff0000));
	ASSERT_NEQ(0xff0000, milk->video.framebuffer[FRAMEBUFFER_POS(1, 1)]);
	TEARDOWN(milk);
}

/*
 *******************************************************************************
 * Audio
 *******************************************************************************
 */

static void _mockLock() /* Should find a nice way to track calls. */
{
}

static void _mockUnlock()
{
}

TEST_CASE(milkSound_WhenIndexOutOfBounds_DoesNothing)
{
	//SETUP(milk);
	//ACT(milkSound(&milk->audio, -1, 0, 0));
	//ASSERT_NULL(milk->audio.queue->next);
	//TEARDOWN(milk);
}

TEST_CASE(milkSound_WhenSampleAtIndexNotLoaded_DoesNothing)
{
	//SETUP(milk);
	//ACT(milkSound(&milk->audio, 0, 0, 0));
	//ASSERT_NULL(milk->audio.queue->next);
	//TEARDOWN(milk);
}

TEST_CASE(milkSound_WhenMaxNumberOfConcurrentSoundsPlaying_DoesNothing)
{
	//SETUP(milk);
	//milk->audio.lock = _mockLock;
	//milk->audio.unlock = _mockUnlock;

	//for (int i = 0; i < MILK_AUDIO_QUEUE_MAX; i++)
	//	milk->audio.queueItems[i].isFree = false;

	//ACT(milkSound(&milk->audio, 0, 0, 0));

	//ASSERT_NULL(milk->audio.queue->next);
	//TEARDOWN(milk);
}

TEST_CASE(milkSound_QueuesNewSample)
{
//	SETUP(milk);
//	milk->audio.lock = _mockLock;
//	milk->audio.unlock = _mockUnlock;
//	uint8_t buffer[] = {7, 7, 7};
//	milk->audio.samples[0].buffer = buffer;
//	milk->audio.samples[0].length = 3;
//
//	ACT(milkSound(&milk->audio, 0, 50, false));
//
//	AudioQueueItem *queuedItem = milk->audio.queue->next;
//	ASSERT_NNULL(queuedItem);
//	ASSERT_EQ(&milk->audio.samples[0], queuedItem->sampleData);
//	ASSERT_EQ(buffer, queuedItem->position);
//	ASSERT_EQ(3, queuedItem->remainingLength);
//	ASSERT_EQ(50, queuedItem->volume);
//	ASSERT_FALSE(queuedItem->isLooping);
//	ASSERT_NULL(queuedItem->next);
//
//CUSTOM_TEARDOWN:
//	milk->audio.samples[0].buffer = NULL; /* We don't want our call to FREE to attempt to free our mock buffer. */
//	FREE_MILK(milk);
}

TEST_CASE(milkSound_WhenLoop_ExistingLoopingSampleIsRemovedFromQueueBeforeQueueingNewSample)
{
//	SETUP(milk);
//	milk->audio.lock = _mockLock;
//	milk->audio.unlock = _mockUnlock;
//	uint8_t buffer[] = { 7, 7, 7 };
//	milk->audio.samples[0].buffer = buffer;
//	milk->audio.samples[0].length = 3;
//	milkSound(&milk->audio, 0, 50, true);
//
//	ACT(milkSound(&milk->audio, 0, 50, true));
//
//	AudioQueueItem *queuedItem = milk->audio.queue->next;
//	ASSERT_NNULL(queuedItem);
//	ASSERT_TRUE(queuedItem->isLooping);
//	ASSERT_NULL(queuedItem->next);
//
//CUSTOM_TEARDOWN:
//	milk->audio.samples[0].buffer = NULL; /* We don't want our call to FREE to attempt to free our mock buffer. */
//	FREE_MILK(milk);
}

TEST_CASE(milkSound_ClampsVolumeToMin)
{
//	SETUP(milk);
//	milk->audio.lock = _mockLock;
//	milk->audio.unlock = _mockUnlock;
//	uint8_t buffer[] = { 7, 7, 7 };
//	milk->audio.samples[0].buffer = buffer;
//	milk->audio.samples[0].length = 3;
//
//	ACT(milkSound(&milk->audio, 0, -10, true));
//
//	AudioQueueItem *queuedItem = milk->audio.queue->next;
//	ASSERT_NNULL(queuedItem);
//	ASSERT_EQ(0, queuedItem->volume);
//
//CUSTOM_TEARDOWN:
//	milk->audio.samples[0].buffer = NULL; /* We don't want our call to FREE to attempt to free our mock buffer. */
//	FREE_MILK(milk);
}

TEST_CASE(milkSound_ClampsVolumeToMax)
{
//	SETUP(milk);
//	milk->audio.lock = _mockLock;
//	milk->audio.unlock = _mockUnlock;
//	uint8_t buffer[] = { 7, 7, 7 };
//	milk->audio.samples[0].buffer = buffer;
//	milk->audio.samples[0].length = 3;
//
//	ACT(milkSound(&milk->audio, 0, MILK_AUDIO_MAX_VOLUME + 10, true));
//
//	AudioQueueItem *queuedItem = milk->audio.queue->next;
//	ASSERT_NNULL(queuedItem);
//	ASSERT_EQ(MILK_AUDIO_MAX_VOLUME, queuedItem->volume);
//
//CUSTOM_TEARDOWN:
//	milk->audio.samples[0].buffer = NULL; /* We don't want our call to FREE to attempt to free our mock buffer. */
//	FREE_MILK(milk);
}

TEST_CASE(milkVolume_ClampsToMinValue)
{
	SETUP(milk);
	ACT(milkVolume(&milk->audio, -10));
	ASSERT_EQ(0, milk->audio.masterVolume);
	TEARDOWN(milk);
}

TEST_CASE(milkVolume_ClampsToMaxValue)
{
	SETUP(milk);
	ACT(milkVolume(&milk->audio, MILK_AUDIO_MAX_VOLUME + 10));
	ASSERT_EQ(MILK_AUDIO_MAX_VOLUME, milk->audio.masterVolume);
	TEARDOWN(milk);
}

TEST_CASE(milkAudioQueueToStream_WhenQueueIsEmpty_DoesNotMixIntoStream)
{
	//SETUP(milk);
	//uint8_t stream[10];
	//ACT(milkAudioQueueToStream(&milk->audio, stream, 10));

	//for (int i = 0; i < 10; i++)
	//	ASSERT_EQ(0, stream[i]);

	//TEARDOWN(milk);
}

TEST_CASE(milkAudioQueueToStream_WhenQueueItemsAreNotFinished_MixesSamplesIntoStream)
{
//	SETUP(milk);
//	milk->audio.lock = _mockLock;
//	milk->audio.unlock = _mockUnlock;
//
//	uint8_t buffer[] = { 7, 7, 7, 7 };
//	milk->audio.samples[0].buffer = buffer;
//	milk->audio.samples[0].length = 4;
//	milkSound(&milk->audio, 0, 128, true);
//
//	uint8_t stream[10];
//	ACT(milkAudioQueueToStream(&milk->audio, stream, 10));
//
//	for (int i = 0; i < 4; i++)
//		ASSERT_EQ(7, stream[i]);
//
//CUSTOM_TEARDOWN:
//	milk->audio.samples[0].buffer = NULL; /* We don't want our call to FREE to attempt to free our mock buffer. */
//	FREE_MILK(milk);
}

TEST_CASE(milkAudioQueueToStream_WhenSamplesIsFinishedAndLooping_ResetsSampleAndMixesIntoStream)
{
//	SETUP(milk);
//	milk->audio.lock = _mockLock;
//	milk->audio.unlock = _mockUnlock;
//
//	uint8_t buffer[] = { 7, 7, 7, 7 };
//	milk->audio.samples[0].buffer = buffer;
//	milk->audio.samples[0].length = 4;
//	milkSound(&milk->audio, 0, 128, true);
//	milk->audio.queue->next->remainingLength = 0;
//
//	uint8_t stream[10];
//	ACT(milkAudioQueueToStream(&milk->audio, stream, 10));
//
//	for (int i = 0; i < 4; i++)
//		ASSERT_EQ(7, stream[i]);
//
//CUSTOM_TEARDOWN:
//	milk->audio.samples[0].buffer = NULL; /* We don't want our call to FREE to attempt to free our mock buffer. */
//	FREE_MILK(milk);
}

TEST_CASE(milkAudioQueueToStream_WhenSamplesIsFinished_RemovesSampleFromQueue)
{
//	SETUP(milk);
//	milk->audio.lock = _mockLock;
//	milk->audio.unlock = _mockUnlock;
//
//	uint8_t buffer[] = { 7, 7, 7, 7 };
//	milk->audio.samples[0].buffer = buffer;
//	milk->audio.samples[0].length = 4;
//	milkSound(&milk->audio, 0, 128, false);
//	milk->audio.queue->next->remainingLength = 0;
//
//	uint8_t stream[10];
//	ACT(milkAudioQueueToStream(&milk->audio, stream, 10));
//	ASSERT_NULL(milk->audio.queue->next);
//
//CUSTOM_TEARDOWN:
//	milk->audio.samples[0].buffer = NULL; /* We don't want our call to FREE to attempt to free our mock buffer. */
//	FREE_MILK(milk);
}

#endif
