#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include <stdbool.h>

// Emulating a gamepad for all input makes the API clean and simple. I do not plan on adding mouse or text input support.
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

// Initializes the input module.
void initInput(Input *input);

// Updates the button state for the gamepad.
// Platform code is responsible for calling this function when system input is polled.
void updateButtonState(Input *input, ButtonState state);

// Returns true if the given button state matches current state.
// This is good for continuous button presses.
bool isButtonDown(Input *input, ButtonState button);

// Returns true if the given button state matches current state and not the previous state.
// This is good for single button presses.
bool isButtonPressed(Input *input, ButtonState button);

#endif
