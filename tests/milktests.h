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

#include <stdlib.h>
#include <string.h>

#include "audio.h"
#include "common.h"
#include "embed/font.h"
#include "gamepad.h"
#include "milkassert.h"
#include "video.h"

#define FRAMEBUFFER_POS(x, y)			((FRAMEBUFFER_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y)	(clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

 /*
  *******************************************************************************
  * Initialization and shutdown
  *******************************************************************************
  */

TEST_CASE(initVideo_InitializesFramebuffer)
{
    Video video;

    ACT(initVideo(&video));

	for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; i++)
		ASSERT_EQ(0x00, video.framebuffer[i]);

	END_ASSERTS();
}

TEST_CASE(initVideo_InitializesSpritesheet)
{
    Video video;

    ACT(initVideo(&video));

    for (int i = 0; i < SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE; i++)
        ASSERT_EQ(0x00, video.spriteSheet[i]);

    END_ASSERTS();
}

TEST_CASE(initVideo_InitializesFont)
{
    Video video;

    ACT(initVideo(&video));

    for (int i = 0; i < FONT_WIDTH * FONT_HEIGHT; i++)
        ASSERT_EQ(DEFAULT_FONT_DATA[i], video.font[i]);

    END_ASSERTS();
}

TEST_CASE(initVideo_InitializesColorKey)
{
    Video video;

    ACT(initVideo(&video));

    ASSERT_EQ(0x00, video.colorKey);
    END_ASSERTS();
}

TEST_CASE(initVideo_InitializesClipRect)
{
    Video video;

    ACT(initVideo(&video));

    ASSERT_EQ(0, video.clipRect.top);
    ASSERT_EQ(0, video.clipRect.left);
    ASSERT_EQ(FRAMEBUFFER_HEIGHT, video.clipRect.bottom);
    ASSERT_EQ(FRAMEBUFFER_WIDTH, video.clipRect.right);
    END_ASSERTS();
}

TEST_CASE(initAudio_InitializesSamples)
{
    Audio audio;

    ACT(initAudio(&audio));

	for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
	{
		ASSERT_NULL(audio.sounds[i].samples);
		ASSERT_EQ(0, audio.sounds[i].length);
	}

    END_ASSERTS();
}

TEST_CASE(initAudio_InitializesAudioSlots)
{
    Audio audio;

    ACT(initAudio(&audio));

	for (int i = 0; i < MAX_SOUND_SLOTS; i++)
	{
		ASSERT_NULL(audio.slots[i].soundData);
		ASSERT_EQ(STOPPED, audio.slots[i].state);
		ASSERT_EQ(0, audio.slots[i].remainingLength);
		ASSERT_NULL(audio.slots[i].position);
		ASSERT_EQ(0, audio.slots[i].volume);
	}

    END_ASSERTS();
}

TEST_CASE(initAudio_InitializesAudioSettings)
{
    Audio audio;

    ACT(initAudio(&audio));

	ASSERT_EQ(MAX_VOLUME, audio.masterVolume);
    END_ASSERTS();
}

/*
 *******************************************************************************
 * Input
 *******************************************************************************
 */

TEST_CASE(isButtonDown_WhenButtonDown_ReturnsTrue)
{
	Input input;
	initInput(&input);
	updateButtonState(&input, BTN_DOWN);

	bool isDown = ACT(isButtonDown(&input, BTN_DOWN));

	ASSERT_TRUE(isDown);
    END_ASSERTS();
}

TEST_CASE(isButtonDown_WhenButtonUp_ReturnsFalse)
{
    Input input;
    initInput(&input);
    updateButtonState(&input, BTN_NONE);

	bool isDown = ACT(isButtonDown(&input, BTN_DOWN));

	ASSERT_FALSE(isDown);
    END_ASSERTS();
}

TEST_CASE(isButtonPressed_WhenPressed_ReturnsTrue)
{
    Input input;
    initInput(&input);
    updateButtonState(&input, BTN_NONE);
    updateButtonState(&input, BTN_DOWN);

	bool isPressed = ACT(isButtonPressed(&input, BTN_DOWN));

	ASSERT_TRUE(isPressed);
    END_ASSERTS();
}

TEST_CASE(isButtonPressed_WhenHeld_ReturnsFalse)
{
    Input input;
    initInput(&input);
    updateButtonState(&input, BTN_DOWN);
    updateButtonState(&input, BTN_DOWN);

    bool isPressed = ACT(isButtonPressed(&input, BTN_DOWN));

    ASSERT_FALSE(isPressed);
    END_ASSERTS();
}

/*
 *******************************************************************************
 * Video
 *******************************************************************************
 */

TEST_CASE(resetDrawState_ResetsColorKey)
{
	Video video;
	initVideo(&video);
	video.colorKey = 0xff;

	ACT(resetDrawState(&video));

	ASSERT_EQ(0x00, video.colorKey);
    END_ASSERTS();
}

TEST_CASE(resetDrawState_ResetsClipRect)
{
    Video video;
    initVideo(&video);

	video.clipRect.top = 10;
	video.clipRect.bottom = 10;
	video.clipRect.left = 10;
	video.clipRect.right = 10;

	ACT(resetDrawState(&video));

	ASSERT_EQ(0, video.clipRect.top);
	ASSERT_EQ(FRAMEBUFFER_HEIGHT, video.clipRect.bottom);
	ASSERT_EQ(0, video.clipRect.left);
	ASSERT_EQ(FRAMEBUFFER_WIDTH, video.clipRect.right);
    END_ASSERTS();
}

TEST_CASE(setClippingRect_ClampsClipRectToFramebufferSize)
{
    Video video;
    initVideo(&video);

	ACT(setClippingRect(&video, -10, -10, 500, 500));

	ASSERT_EQ(0, video.clipRect.top);
	ASSERT_EQ(FRAMEBUFFER_HEIGHT, video.clipRect.bottom);
	ASSERT_EQ(0, video.clipRect.left);
	ASSERT_EQ(FRAMEBUFFER_WIDTH, video.clipRect.right);
    END_ASSERTS();
}

TEST_CASE(setClippingRect_SetsClipRect)
{
    Video video;
    initVideo(&video);

	ACT(setClippingRect(&video, 10, 20, 200, 100));

	ASSERT_EQ(20, video.clipRect.top);
	ASSERT_EQ(120, video.clipRect.bottom);
	ASSERT_EQ(10, video.clipRect.left);
	ASSERT_EQ(210, video.clipRect.right);
    END_ASSERTS();
}

TEST_CASE(blitPixel_WhenPixelWithinClipRect_SetsPixel)
{
    Video video;
    initVideo(&video);
	clearFramebuffer(&video, 0x00);

	ACT(blitPixel(&video, 15, 30, 0x00ff00));

	ASSERT_EQ(0x00ff00, video.framebuffer[FRAMEBUFFER_POS(15, 30)]);
    END_ASSERTS();
}

TEST_CASE(blitPixel_WhenPixelIsNotWithinClipRect_DoesNotSetPixel)
{
    Video video;
    initVideo(&video);
	clearFramebuffer(&video, 0x00);
	setClippingRect(&video, 10, 20, 200, 100);

	ACT(blitPixel(&video, 1, 1, 0xff0000));

	ASSERT_NEQ(0xff0000, video.framebuffer[FRAMEBUFFER_POS(1, 1)]);
    END_ASSERTS();
}

/*
 *******************************************************************************
 * Audio
 *******************************************************************************
 */

static void mockLock() /* Should find a nice way to track calls. */
{
}

static void mockUnlock()
{
}

TEST_CASE(playSound_WhenIndexOutOfBounds_DoesNothing)
{
    Audio audio;
    initAudio(&audio);

	ACT(playSound(&audio, -1, 0, 0));
	ACT(playSound(&audio, MAX_LOADED_SOUNDS + 1, 0, 0));
}

TEST_CASE(playSound_WhenSampleAtIndexNotLoaded_DoesNothing)
{
    Audio audio;
    initAudio(&audio);

	ACT(playSound(&audio, 0, -1, 0));
	ACT(playSound(&audio, MAX_SOUND_SLOTS + 1, 0, 0));
}

TEST_CASE(playSound_WhenSampleLengthIsZero_DoesNothing)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;
	audio.sounds[0].length = 0;

	ACT(playSound(&audio, 0, 0, 0));

	ASSERT_NEQ(&audio.sounds[0], audio.slots[0].soundData);
	END_ASSERTS();
}

TEST_CASE(playSound_SetsSlot)
{
    Audio audio;
    initAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	uint8_t buffer[1];
	audio.sounds[0].samples = buffer;
	audio.sounds[0].length = 1;

	ACT(playSound(&audio, 0, 0, 50));

	ASSERT_EQ(&audio.sounds[0], audio.slots[0].soundData);
	ASSERT_EQ(buffer, audio.slots[0].position);
	ASSERT_EQ(1, audio.slots[0].remainingLength);
	ASSERT_EQ(PLAYING, audio.slots[0].state);
	ASSERT_EQ(50, audio.slots[0].volume);
	END_ASSERTS();
}

TEST_CASE(playSound_ClampsVolumeToMin)
{
    Audio audio;
    initAudio(&audio);

    uint8_t buffer[1];
	audio.lock = mockLock;
	audio.unlock = mockUnlock;
    audio.sounds[0].samples = buffer;

	ACT(playSound(&audio, 0, 0, -10));

	ASSERT_EQ(0, audio.slots[0].volume);
    END_ASSERTS();
}

TEST_CASE(playSound_ClampsVolumeToMax)
{
    Audio audio;
    initAudio(&audio);

    uint8_t buffer[1];
	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.sounds[0].samples = buffer;

	ACT(playSound(&audio, 0, 0, MAX_VOLUME + 10));

	ASSERT_EQ(MAX_VOLUME, audio.slots[0].volume);
	END_ASSERTS();
}

TEST_CASE(setMasterVolume_ClampsToMinValue)
{
    Audio audio;
    initAudio(&audio);

	ACT(setMasterVolume(&audio, -10));
	ASSERT_EQ(0, audio.masterVolume);
    END_ASSERTS();
}

TEST_CASE(setMasterVolume_ClampsToMaxValue)
{
    Audio audio;
    initAudio(&audio);

	ACT(setMasterVolume(&audio, MAX_VOLUME + 10));
	ASSERT_EQ(MAX_VOLUME, audio.masterVolume);
    END_ASSERTS();
}

TEST_CASE(stopSound_WhenIndexOutOfBounds_DoesNothing)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;
	ACT(stopSound(&audio, -10));
	ACT(stopSound(&audio, MAX_SOUND_SLOTS + 10));
}

TEST_CASE(stopSound_WhenIndexIsNegativeOne_StopsAllPlayingSounds)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        audio.slots[i].state = PLAYING;

    ACT(stopSound(&audio, -1));

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        ASSERT_EQ(STOPPED, audio.slots[i].state);

    END_ASSERTS();
}

TEST_CASE(stopSound_StopsSound)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;
	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.slots[0].soundData = &audio.sounds[0];
	audio.slots[0].state = PLAYING;

	ACT(stopSound(&audio, 0));

	ASSERT_NULL(audio.slots[0].soundData);
	ASSERT_EQ(STOPPED, audio.slots[0].state);

    END_ASSERTS();
}

TEST_CASE(pauseSound_WhenIndexOutOfBounds_DoesNothing)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;
	ACT(pauseSound(&audio, -10));
	ACT(pauseSound(&audio, MAX_SOUND_SLOTS + 10));
}

TEST_CASE(pauseSound_WhenIndexIsNegativeOne_PausesAllPlayingSounds)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        audio.slots[i].state = PLAYING;

    ACT(pauseSound(&audio, -1));

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        ASSERT_EQ(PAUSED, audio.slots[i].state);
    END_ASSERTS();
}

TEST_CASE(pauseSound_PausesSound)
{
    Audio audio;
    initAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.slots[0].soundData = &audio.sounds[0];
	audio.slots[0].state = PLAYING;

	ACT(pauseSound(&audio, 0));

	ASSERT_EQ(PAUSED, audio.slots[0].state);
    END_ASSERTS();
}

TEST_CASE(resumeSound_WhenIndexOutOfBounds_DoesNothing)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;
	ACT(resumeSound(&audio, -10));
	ACT(resumeSound(&audio, MAX_SOUND_SLOTS + 10));
}

TEST_CASE(resumeSound_WhenIndexIsNegativeOne_PausesAllPausedSounds)
{
    Audio audio;
    initAudio(&audio);

    audio.lock = mockLock;
    audio.unlock = mockUnlock;

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        audio.slots[i].state = PAUSED;

    ACT(resumeSound(&audio, -1));

    for (int i = 0; i < MAX_SOUND_SLOTS; i++)
        ASSERT_EQ(PLAYING, audio.slots[i].state);
    END_ASSERTS();
}

TEST_CASE(resumeSound_resumesSound)
{
    Audio audio;
    initAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.slots[0].soundData = &audio.sounds[0];
	audio.slots[0].state = PAUSED;

	ACT(resumeSound(&audio, 0));

	ASSERT_EQ(PLAYING, audio.slots[0].state);
    END_ASSERTS();
}

#endif
