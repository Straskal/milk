#include "milk.h"

#include <assert.h>
#include <stdlib.h>

/* Test helpers */
#define TEST_METHOD(test)				static void test()
#define ARRANGE_MILK(milk)				Milk *milk = milkCreate()
#define FREE_MILK(milk)					milkFree(milk)

/* Asserts */
#define ASSERT_TRUE(condition)			assert(condition)
#define ASSERT_FALSE(condition)			assert(!(condition))
#define ASSERT_EQ(expected, actual)		assert(expected == actual)
#define ASSERT_NEQ(expected, actual)	assert(expected != actual)
#define ASSERT_NOT_NULL(val)			assert(val != NULL)

typedef void(*TestFunction)();

typedef struct test
{
	char *name;
	TestFunction testFunc;
} Test;

TEST_METHOD(milkCreate_InitializesMilk)
{
	ARRANGE_MILK(milk);
	ASSERT_NOT_NULL(milk->audio.queue);
	ASSERT_EQ(MILK_AUDIO_MAX_VOLUME, milk->audio.masterVolume);

	for (int i = 0; i < MILK_AUDIO_QUEUE_MAX; i++)
		ASSERT_TRUE(milk->audio.queueItems[i].isFree);

	FREE_MILK(milk);
}

TEST_METHOD(milkButton_ReturnsExpectedResult)
{
	ARRANGE_MILK(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;
	ASSERT_TRUE(milkButton(&milk->input, BTN_DOWN));
	milk->input.gamepad.buttonState = 0;
	ASSERT_FALSE(milkButton(&milk->input, BTN_DOWN));
	FREE_MILK(milk);
}

TEST_METHOD(milkButtonPressed_ReturnsExpectedResult)
{
	ARRANGE_MILK(milk);
	milk->input.gamepad.buttonState |= BTN_DOWN;
	milk->input.gamepad.previousButtonState = 0;
	ASSERT_TRUE(milkButtonPressed(&milk->input, BTN_DOWN));
	milk->input.gamepad.buttonState |= BTN_DOWN;
	milk->input.gamepad.previousButtonState |= BTN_DOWN;
	ASSERT_FALSE(milkButtonPressed(&milk->input, BTN_DOWN));
	FREE_MILK(milk);
}

TEST_METHOD(milkResetDrawState_ResetsColorKey)
{
	ARRANGE_MILK(milk);
	milk->video.colorKey = 57;
	milkResetDrawState(&milk->video);
	ASSERT_EQ(0x00, milk->video.colorKey);
	FREE_MILK(milk);
}

TEST_METHOD(milkResetDrawState_ResetsClipRect)
{
	ARRANGE_MILK(milk);
	milk->video.clipRect.top = 10;
	milk->video.clipRect.bottom = 10;
	milk->video.clipRect.left = 10;
	milk->video.clipRect.right = 10;
	milkResetDrawState(&milk->video);
	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(MILK_FRAMEBUF_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(MILK_FRAMEBUF_WIDTH, milk->video.clipRect.right);
	FREE_MILK(milk);
}

TEST_METHOD(milkClipRect_LimitsClipRectToFramebufferSize)
{
	ARRANGE_MILK(milk);
	milkClipRect(&milk->video, -10, -10, 500, 500);
	ASSERT_EQ(0, milk->video.clipRect.top);
	ASSERT_EQ(MILK_FRAMEBUF_HEIGHT, milk->video.clipRect.bottom);
	ASSERT_EQ(0, milk->video.clipRect.left);
	ASSERT_EQ(MILK_FRAMEBUF_WIDTH, milk->video.clipRect.right);
	FREE_MILK(milk);
}

TEST_METHOD(milkClipRect_SetsClipRect)
{
	ARRANGE_MILK(milk);
	milkClipRect(&milk->video, 10, 20, 200, 100);
	ASSERT_EQ(20, milk->video.clipRect.top);
	ASSERT_EQ(120, milk->video.clipRect.bottom);
	ASSERT_EQ(10, milk->video.clipRect.left);
	ASSERT_EQ(210, milk->video.clipRect.right);
	FREE_MILK(milk);
}

#define FRAMEBUFFER_POS(x, y)			((MILK_FRAMEBUF_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y)	(clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

TEST_METHOD(milkClear_SetsPixelsWithinClipRect)
{
	ARRANGE_MILK(milk);
	milkClipRect(&milk->video, 0, 0, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);
	milkClear(&milk->video, 0xff0000);
	milkClipRect(&milk->video, 10, 20, 200, 100);
	milkClear(&milk->video, 0x00ff00);

	for (int i = 0; i < MILK_FRAMEBUF_HEIGHT; i++)
	{
		for (int j = 0; j < MILK_FRAMEBUF_HEIGHT; j++)
		{
			Color32 color = milk->video.framebuffer[FRAMEBUFFER_POS(j, i)];

			if (WITHIN_CLIP_RECT(milk->video.clipRect, j, i))
				ASSERT_EQ(0x00ff00, color);
			else
				ASSERT_EQ(0xff0000, color);
		}
	}

	FREE_MILK(milk);
}

TEST_METHOD(milkPixelSet_SetsPixelWithinClipRect)
{
	ARRANGE_MILK(milk);
	milkClipRect(&milk->video, 0, 0, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);
	milkClear(&milk->video, 0x000000);
	milkClipRect(&milk->video, 10, 20, 200, 100);
	milkPixelSet(&milk->video, 1, 1, 0xff0000);
	milkPixelSet(&milk->video, 15, 30, 0x00ff00);
	ASSERT_NEQ(0xff0000, milk->video.framebuffer[FRAMEBUFFER_POS(1, 1)]);
	ASSERT_EQ(0x00ff00, milk->video.framebuffer[FRAMEBUFFER_POS(15, 30)]);
}

static Test _tests[] =
{
	#define TEST(func) { #func, func }
	TEST(milkCreate_InitializesMilk),
	TEST(milkButton_ReturnsExpectedResult),
	TEST(milkButtonPressed_ReturnsExpectedResult),
	TEST(milkResetDrawState_ResetsColorKey),
	TEST(milkResetDrawState_ResetsClipRect),
	TEST(milkClipRect_LimitsClipRectToFramebufferSize),
	TEST(milkClipRect_SetsClipRect),
	TEST(milkClear_SetsPixelsWithinClipRect),
	TEST(milkPixelSet_SetsPixelWithinClipRect)
	#undef TEST
};

int main(int argc, char *argv[])
{
	const size_t testCount = sizeof(_tests) / sizeof(Test);

	for (size_t i = 0; i < testCount; i++)
		_tests[i].testFunc();
}
