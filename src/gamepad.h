#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include <stdbool.h>

typedef enum {
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

typedef enum {
  INPUT_NONE    = 0 << 0,
  INPUT_CHAR    = 1 << 0,
  INPUT_BACK    = 1 << 1,
  INPUT_ENTER   = 1 << 2,
  INPUT_ESCAPE  = 1 << 3
} ExtendedInputState;

typedef struct {
  ExtendedInputState state;
  ExtendedInputState previousState;
  char inChar;
} ExtendedInput;

typedef struct {
  ButtonState buttonState;
  ButtonState previousButtonState;
} Gamepad;

typedef struct {
  Gamepad gamepad;
  ExtendedInput extended;
} Input;

void initializeInput(Input *input);
void updateButtonState(Input *input, ButtonState state);
void updateExtendedInputState(Input *input, ExtendedInputState state, char inChar);
bool isButtonDown(Input *input, ButtonState button);
bool isButtonPressed(Input *input, ButtonState button);
bool isExtDown(Input *input, ExtendedInputState in);
bool isExtPressed(Input *input, ExtendedInputState in);

#endif
