#include "common.h"
#include "gamepad.h"

void initInput(Input *input)
{
  input->gamepad.buttonState = BTN_NONE;
  input->gamepad.previousButtonState = BTN_NONE;
}

void updateButtonState(Input *input, ButtonState state)
{
  input->gamepad.previousButtonState = input->gamepad.buttonState;
  input->gamepad.buttonState = state;
}

bool isButtonDown(Input *input, ButtonState button)
{
  return IS_BIT_SET(input->gamepad.buttonState, button);
}

bool isButtonPressed(Input *input, ButtonState button)
{
  return IS_BIT_SET(input->gamepad.buttonState, button) && !IS_BIT_SET(input->gamepad.previousButtonState, button);
}
