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

#endif
