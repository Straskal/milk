#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include <stdbool.h>

 /********************************************************************************
	* Specification:
	* - [9] buttons to emulate a basic gamepad.
	* - There are keyboard mappings.
	* - Buttons can be either up or down.
	*
	* Notes:
	* Milk's input is pretty bare bones. Nothing fancy.
	* Emulating a gamepad keeps the API simple. I don't plan on adding mouse or text input support.
	*******************************************************************************/

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

/**
 * Initialize the input.
 * @param input
 */
void initInput(Input *input);

/**
 * Update the button state of the gamepad.
 * @param input
 */
void updateButtonState(Input *input, ButtonState state);

/**
 * Returns true if the given button state matches current state.
 * This is good for continuous button presses.
 * @param input
 * @param button
 * @return
 */
bool isButtonDown(Input *input, ButtonState button);

/**
 * Returns true if the given button state matches current state and not the previous state.
 * This is good for single button presses.
 * @param input
 * @param button
 * @return
 */
bool isButtonPressed(Input *input, ButtonState button);

#endif
