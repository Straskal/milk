#ifndef __INPUT_TESTS_H__
#define __INPUT_TESTS_H__

#include "core/milkassert.h"

TEST_CASE(isButtonDown_WhenButtonDown_ReturnsTrue) {
  Input input;
  initInput(&input);
  updateButtonState(&input, BTN_DOWN);
  bool isDown = ACT(isButtonDown(&input, BTN_DOWN));
  ASSERT_TRUE(isDown);
  END_ASSERTS();
}

TEST_CASE(isButtonDown_WhenButtonUp_ReturnsFalse) {
  Input input;
  initInput(&input);
  updateButtonState(&input, BTN_NONE);
  bool isDown = ACT(isButtonDown(&input, BTN_DOWN));
  ASSERT_FALSE(isDown);
  END_ASSERTS();
}

TEST_CASE(isButtonPressed_WhenPressed_ReturnsTrue) {
  Input input;
  initInput(&input);
  updateButtonState(&input, BTN_NONE);
  updateButtonState(&input, BTN_DOWN);
  bool isPressed = ACT(isButtonPressed(&input, BTN_DOWN));
  ASSERT_TRUE(isPressed);
  END_ASSERTS();
}

TEST_CASE(isButtonPressed_WhenHeld_ReturnsFalse) {
  Input input;
  initInput(&input);
  updateButtonState(&input, BTN_DOWN);
  updateButtonState(&input, BTN_DOWN);
  bool isPressed = ACT(isButtonPressed(&input, BTN_DOWN));
  ASSERT_FALSE(isPressed);
  END_ASSERTS();
}

#endif
