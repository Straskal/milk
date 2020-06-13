/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef __MILK_TESTS_H__
#define __MILK_TESTS_H__

#include "milk.h"
#include "milkassert.h"
#include "embed/font.h"

#include <string.h>

#define FRAMEBUFFER_POS(x, y)			((FRAMEBUFFER_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y)	(clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

 /*
  *******************************************************************************
  * Initialization and shutdown
  *******************************************************************************
  */

TEST_CASE(createMilk_InitializesFramebuffer)
{
	SETUP(milk);

	for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; i++)
		ASSERT_EQ(0x00, milk->video.framebuffer[i]);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(createMilk_InitializesSpritesheet)
{
	SETUP(milk);

	for (int i = 0; i < SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE; i++)
		ASSERT_EQ(0x00, milk->video.spriteSheet[i]);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(createMilk_InitializesFont)
{
	SETUP(milk);

	for (int i = 0; i < FONT_WIDTH * FONT_HEIGHT; i++)
		ASSERT_EQ(DEFAULT_FONT_DATA[i], milk->video.font[i]);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(createMilk_InitializesColorKey)
{
	SETUP(milk);
	ASSERT_EQ(0x00, milk->video.colorKey);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(createMilk_InitializesClipRect)
{
	SETUP(milk);
	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(FRAMEBUFFER_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(FRAMEBUFFER_WIDTH, milk->video.clipRect.right);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(createMilk_InitializesSamples)
{
	SETUP(milk);

	for (int i = 0; i < MAX_LOADED_SAMPLES; i++)
	{
		ASSERT_NULL(milk->audio.samples[i].buffer);
		ASSERT_EQ(0, milk->audio.samples[i].length);
	}

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(createMilk_InitializesAudioSlots)
{
	SETUP(milk);

	for (int i = 0; i < MAX_SAMPLE_SLOTS; i++)
	{
		ASSERT_NULL(milk->audio.slots[i].sampleData);
		ASSERT_EQ(STOPPED, milk->audio.slots[i].state);
		ASSERT_EQ(0, milk->audio.slots[i].remainingLength);
		ASSERT_NULL(milk->audio.slots[i].position);
		ASSERT_EQ(0, milk->audio.slots[i].volume);
	}

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(createMilk_InitializesAudioSettings)
{
	SETUP(milk);
	ASSERT_EQ(MAX_VOLUME, milk->audio.masterVolume);
	ASSERT_EQ(0, milk->audio.frequency);
	ASSERT_EQ(0, milk->audio.channels);

TEARDOWN:
	FREE_MILK(milk);
}

/*
 *******************************************************************************
 * Input
 *******************************************************************************
 */

TEST_CASE(isButtonDown_WhenButtonDown_ReturnsTrue)
{
	SETUP(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;

	bool isDown = ACT(isButtonDown(&milk->input, BTN_DOWN));

	ASSERT_TRUE(isDown);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(isButtonDown_WhenButtonUp_ReturnsFalse)
{
	SETUP(milk);
	milk->input.gamepad.buttonState = BTN_NONE;

	bool isDown = ACT(isButtonDown(&milk->input, BTN_DOWN));

	ASSERT_FALSE(isDown);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(isButtonPressed_WhenPressed_ReturnsTrue)
{
	SETUP(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;
	milk->input.gamepad.previousButtonState = BTN_NONE;

	bool isPressed = ACT(isButtonPressed(&milk->input, BTN_DOWN));

	ASSERT_TRUE(isPressed);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(isButtonPressed_WhenHeld_ReturnsFalse)
{
	SETUP(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;
	milk->input.gamepad.previousButtonState |= BTN_DOWN;

	bool isPressed = ACT(isButtonPressed(&milk->input, BTN_DOWN));

	ASSERT_FALSE(isPressed);

TEARDOWN:
	FREE_MILK(milk);
}

/*
 *******************************************************************************
 * Video
 *******************************************************************************
 */

TEST_CASE(resetDrawState_ResetsColorKey)
{
	SETUP(milk);
	milk->video.colorKey = 57;

	ACT(resetDrawState(&milk->video));

	ASSERT_EQ(0x00, milk->video.colorKey);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(resetDrawState_ResetsClipRect)
{
	SETUP(milk);

	milk->video.clipRect.top = 10;
	milk->video.clipRect.bottom = 10;
	milk->video.clipRect.left = 10;
	milk->video.clipRect.right = 10;

	ACT(resetDrawState(&milk->video));

	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(FRAMEBUFFER_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(FRAMEBUFFER_WIDTH, milk->video.clipRect.right);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(setClippingRect_ClampsClipRectToFramebufferSize)
{
	SETUP(milk);

	ACT(setClippingRect(&milk->video, -10, -10, 500, 500));

	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(FRAMEBUFFER_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(FRAMEBUFFER_WIDTH, milk->video.clipRect.right);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(setClippingRect_SetsClipRect)
{
	SETUP(milk);

	ACT(setClippingRect(&milk->video, 10, 20, 200, 100));

	ASSERT_EQ(20, milk->video.clipRect.top);
	ASSERT_EQ(120, milk->video.clipRect.bottom);
	ASSERT_EQ(10, milk->video.clipRect.left);
	ASSERT_EQ(210, milk->video.clipRect.right);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(blitPixel_WhenPixelWithinClipRect_SetsPixel)
{
	SETUP(milk);
	clearFramebuffer(&milk->video, 0x00);
	ACT(blitPixel(&milk->video, 15, 30, 0x00ff00));
	ASSERT_EQ(0x00ff00, milk->video.framebuffer[FRAMEBUFFER_POS(15, 30)]);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(blitPixel_WhenPixelIsNotWithinClipRect_DoesNotSetPixel)
{
	SETUP(milk);
	clearFramebuffer(&milk->video, 0x00);
	setClippingRect(&milk->video, 10, 20, 200, 100);
	ACT(blitPixel(&milk->video, 1, 1, 0xff0000));
	ASSERT_NEQ(0xff0000, milk->video.framebuffer[FRAMEBUFFER_POS(1, 1)]);

TEARDOWN:
	FREE_MILK(milk);
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

TEST_CASE(playSound_WhenIndexOutOfBounds_DoesNothing)
{
	SETUP(milk);
	ACT(playSound(&milk->audio, -1, 0, 0));
	ACT(playSound(&milk->audio, MAX_LOADED_SAMPLES + 1, 0, 0));
	FREE_MILK(milk);
}

TEST_CASE(playSound_WhenSampleAtIndexNotLoaded_DoesNothing)
{
	SETUP(milk);
	ACT(playSound(&milk->audio, 0, -1, 0));
	ACT(playSound(&milk->audio, MAX_SAMPLE_SLOTS + 1, 0, 0));
}

TEST_CASE(playSound_WhenSampleLengthIsZero_DoesNothing)
{
	SETUP(milk);
	milk->audio.samples[0].length = 0;

	ACT(playSound(&milk->audio, 0, 0, 0));

	ASSERT_NEQ(&milk->audio.samples[0], milk->audio.slots[0].sampleData);

	milk->audio.samples[0].buffer = NULL;

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(playSound_SetsSlot)
{
	SETUP(milk);
	milk->audio.lock = _mockLock;
	milk->audio.unlock = _mockUnlock;
	uint8_t buffer[1];
	milk->audio.samples[0].buffer = buffer;
	milk->audio.samples[0].length = 1;

	ACT(playSound(&milk->audio, 0, 0, 50));

	ASSERT_EQ(&milk->audio.samples[0], milk->audio.slots[0].sampleData);
	ASSERT_EQ(buffer, milk->audio.slots[0].position);
	ASSERT_EQ(1, milk->audio.slots[0].remainingLength);
	ASSERT_EQ(PLAYING, milk->audio.slots[0].state);
	ASSERT_EQ(50, milk->audio.slots[0].volume);

	milk->audio.samples[0].buffer = NULL;

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(playSound_ClampsVolumeToMin)
{
	SETUP(milk);
	milk->audio.lock = _mockLock;
	milk->audio.unlock = _mockUnlock;
	milk->audio.samples[0].length = 1;

	ACT(playSound(&milk->audio, 0, 0, -10));

	ASSERT_EQ(0, milk->audio.slots[0].volume);

	milk->audio.samples[0].buffer = NULL;

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(playSound_ClampsVolumeToMax)
{
	SETUP(milk);
	milk->audio.lock = _mockLock;
	milk->audio.unlock = _mockUnlock;
	milk->audio.samples[0].length = 1;

	ACT(playSound(&milk->audio, 0, 0, MAX_VOLUME + 10));

	ASSERT_EQ(MAX_VOLUME, milk->audio.slots[0].volume);

	milk->audio.samples[0].buffer = NULL;

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(setMasterVolume_ClampsToMinValue)
{
	SETUP(milk);
	ACT(setMasterVolume(&milk->audio, -10));
	ASSERT_EQ(0, milk->audio.masterVolume);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(setMasterVolume_ClampsToMaxValue)
{
	SETUP(milk);
	ACT(setMasterVolume(&milk->audio, MAX_VOLUME + 10));
	ASSERT_EQ(MAX_VOLUME, milk->audio.masterVolume);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(stopSound_WhenIndexOutOfBounds_DoesNothing)
{
	SETUP(milk);
	ACT(stopSound(&milk->audio, -10));
	ACT(stopSound(&milk->audio, MAX_SAMPLE_SLOTS + 10));
	FREE_MILK(milk);
}

TEST_CASE(stopSound_StopsSound)
{
	SETUP(milk);
	milk->audio.lock = _mockLock;
	milk->audio.unlock = _mockUnlock;
	milk->audio.slots[0].sampleData = &milk->audio.samples[0];
	milk->audio.slots[0].state = PLAYING;

	ACT(stopSound(&milk->audio, 0));

	ASSERT_NULL(milk->audio.slots[0].sampleData);
	ASSERT_EQ(STOPPED, milk->audio.slots[0].state);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(pauseSound_WhenIndexOutOfBounds_DoesNothing)
{
	SETUP(milk);
	ACT(pauseSound(&milk->audio, -10));
	ACT(pauseSound(&milk->audio, MAX_SAMPLE_SLOTS + 10));
	FREE_MILK(milk);
}

TEST_CASE(pauseSound_PausesSound)
{
	SETUP(milk);
	milk->audio.lock = _mockLock;
	milk->audio.unlock = _mockUnlock;
	milk->audio.slots[0].sampleData = &milk->audio.samples[0];
	milk->audio.slots[0].state = PLAYING;

	ACT(pauseSound(&milk->audio, 0));

	ASSERT_EQ(PAUSED, milk->audio.slots[0].state);

TEARDOWN:
	FREE_MILK(milk);
}

TEST_CASE(resumeSound_WhenIndexOutOfBounds_DoesNothing)
{
	SETUP(milk);
	ACT(resumeSound(&milk->audio, -10));
	ACT(resumeSound(&milk->audio, MAX_SAMPLE_SLOTS + 10));
	FREE_MILK(milk);
}

TEST_CASE(resumeSound_resumesSound)
{
	SETUP(milk);
	milk->audio.lock = _mockLock;
	milk->audio.unlock = _mockUnlock;
	milk->audio.slots[0].sampleData = &milk->audio.samples[0];
	milk->audio.slots[0].state = PAUSED;

	ACT(resumeSound(&milk->audio, 0));

	ASSERT_EQ(PLAYING, milk->audio.slots[0].state);

TEARDOWN:
	FREE_MILK(milk);
}

#endif
