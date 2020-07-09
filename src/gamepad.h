#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include <stdbool.h>

typedef enum buttonState
{
  BTN_NONE  = 0 << 0,
  BTN_START = 1 << 0,
  BTN_UP    = 1 << 1,
  BTN_DOWN  = 1 << 2,
  BTN_LEFT  = 1 << 3,
  BTN_RIGHT = 1 << 4,
  BTN_A     = 1 << 5,
  BTN_B     = 1 << 6,
  BTN_X     = 1 << 7,
  BTN_Y     = 1 << 8,
} ButtonState;

typedef struct gamepad
{
  ButtonState buttonState;
  ButtonState previousButtonState;
} Gamepad;

typedef struct input
{
  Gamepad gamepad;
} Input;

void initInput(Input *input);
void updateButtonState(Input *input, ButtonState state);
bool isButtonDown(Input *input, ButtonState button);
bool isButtonPressed(Input *input, ButtonState button);

#endif
