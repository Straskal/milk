#include <string.h>

#include "common.h"
#include "input.h"

void initializeInput(Input *input)
{
  memset(input, 0, sizeof(Input));
}

bool isButtonDown(Input *input, ButtonState button)
{
  return CHECK_BIT(input->gamepad.state, 1 << button);
}

bool isButtonPressed(Input *input, ButtonState button)
{
  return CHECK_BIT(input->gamepad.state, 1 << button) && !CHECK_BIT(input->gamepad.previousState, 1 << button);
}

bool isKeyDown(Input *input, Key key)
{
  return input->keyboard.state[key];
}

bool isKeyPressed(Input *input, Key key)
{
  return input->keyboard.state[key] && !input->keyboard.previousState[key];
}

bool isMouseDown(Input *input, MouseState key)
{
  return CHECK_BIT(input->mouse.state, key);
}

bool isMousePressed(Input *input, MouseState key)
{
  return CHECK_BIT(input->mouse.state, key) && !CHECK_BIT(input->mouse.previousState, key);
}
