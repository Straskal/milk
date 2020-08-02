#include <string.h>

#include "common.h"
#include "gamepad.h"

void initializeInput(Input *input) {
  memset(input, 0, sizeof(Input));
}

void updateButtonState(Input *input, ButtonState state) {
  input->gamepad.previousButtonState = input->gamepad.buttonState;
  input->gamepad.buttonState = state;
}

void updateExtendedInputState(Input *input, ExtendedInputState state, char inChar) {
  input->extended.previousState = input->extended.state;
  input->extended.state = state;
  input->extended.inChar = inChar;
}

bool isButtonDown(Input *input, ButtonState button) {
  return IS_BIT_SET(input->gamepad.buttonState, button);
}

bool isButtonPressed(Input *input, ButtonState button) {
  return IS_BIT_SET(input->gamepad.buttonState, button) && !IS_BIT_SET(input->gamepad.previousButtonState, button);
}

bool isExtDown(Input *input, ExtendedInputState in) {
  return IS_BIT_SET(input->extended.state, in);
}

bool isExtPressed(Input *input, ExtendedInputState in) {
  return IS_BIT_SET(input->extended.state, in) && !IS_BIT_SET(input->extended.previousState, in);
}
