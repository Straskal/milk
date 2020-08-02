#include <string.h>

#include "common.h"
#include "gamepad.h"

void initializeInput(Input *input)
{
  memset(input, 0, sizeof(Input));
}

void updateButtonState(Input *input, ButtonState state)
{
  input->gamepad.previousButtonState = input->gamepad.buttonState;
  input->gamepad.buttonState = state;
}

void updateExtendedInputState(Input *input, ExtendedInputState state, char inChar)
{
  input->extended.previousState = input->extended.state;
  input->extended.state = state;
  input->extended.inChar = inChar;
}

bool isButtonDown(Input *input, ButtonState button)
{
  return CHECK_BIT(input->gamepad.buttonState, button);
}

bool isButtonPressed(Input *input, ButtonState button)
{
  return CHECK_BIT(input->gamepad.buttonState, button) && !CHECK_BIT(input->gamepad.previousButtonState, button);
}

bool isExtDown(Input *input, ExtendedInputState in)
{
  return CHECK_BIT(input->extended.state, in);
}

bool isExtPressed(Input *input, ExtendedInputState in)
{
  return CHECK_BIT(input->extended.state, in) && !CHECK_BIT(input->extended.previousState, in);
}
