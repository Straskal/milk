#ifndef __VIDEO_TESTS_H__
#define __VIDEO_TESTS_H__

#include "core/milkassert.h"

#define FRAMEBUFFER_POS(x, y) ((FRAMEBUFFER_WIDTH * y) + x)
#define WITHIN_CLIP_RECT(clip, x, y) (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom)

TEST_CASE(initVideo_InitializesFramebuffer)
{
  Video video;

  ACT(initializeVideo(&video));

  for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; i++)
    ASSERT_EQ(0x00, video.framebuffer[i]);

  END_ASSERTS();
}

TEST_CASE(initVideo_InitializesSpritesheet)
{
  Video video;

  ACT(initializeVideo(&video));

  for (int i = 0; i < SPRITE_SHEET_SQRSIZE * SPRITE_SHEET_SQRSIZE; i++)
    ASSERT_EQ(0x00, video.spriteSheet[i]);

  END_ASSERTS();
}

TEST_CASE(initVideo_InitializesColorKey)
{
  Video video;

  ACT(initializeVideo(&video));

  ASSERT_EQ(0x00, video.colorKey);
  END_ASSERTS();
}

TEST_CASE(initVideo_InitializesClipRect)
{
  Video video;

  ACT(initializeVideo(&video));

  ASSERT_EQ(0, video.clipRect.top);
  ASSERT_EQ(0, video.clipRect.left);
  ASSERT_EQ(FRAMEBUFFER_HEIGHT, video.clipRect.bottom);
  ASSERT_EQ(FRAMEBUFFER_WIDTH, video.clipRect.right);
  END_ASSERTS();
}

TEST_CASE(resetDrawState_ResetsColorKey)
{
  Video video;
  initializeVideo(&video);
  video.colorKey = 0xff;

  ACT(resetDrawState(&video));

  ASSERT_EQ(0x00, video.colorKey);
  END_ASSERTS();
}

TEST_CASE(resetDrawState_ResetsClipRect)
{
  Video video;
  initializeVideo(&video);

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
  initializeVideo(&video);

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
  initializeVideo(&video);

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
  initializeVideo(&video);
  clearFramebuffer(&video, 0x00);

  ACT(blitPixel(&video, 15, 30, 0x00ff00));

  ASSERT_EQ(0x00ff00, video.framebuffer[FRAMEBUFFER_POS(15, 30)]);
  END_ASSERTS();
}

TEST_CASE(blitPixel_WhenPixelIsNotWithinClipRect_DoesNotSetPixel)
{
  Video video;
  initializeVideo(&video);
  clearFramebuffer(&video, 0x00);
  setClippingRect(&video, 10, 20, 200, 100);

  ACT(blitPixel(&video, 1, 1, 0xff0000));

  ASSERT_NEQ(0xff0000, video.framebuffer[FRAMEBUFFER_POS(1, 1)]);
  END_ASSERTS();
}

#endif
