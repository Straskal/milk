/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software andassociated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, andto permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice andthis permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef __MILK_H__
#define __MILK_H__

#include "milk_audio.h"
#include "milk_video.h"

#include <stdint.h>

#define MILK_BOOL int
#define MILK_TRUE 1
#define MILK_FALSE 0

enum
{
	BTN_UP      = 1 << 0,
	BTN_DOWN    = 1 << 1,
	BTN_LEFT    = 1 << 2,
	BTN_RIGHT   = 1 << 3,
	BTN_A       = 1 << 4,
	BTN_B       = 1 << 5,
	BTN_X       = 1 << 6,
	BTN_Y       = 1 << 7
};

typedef struct Gamepad
{
	uint8_t buttonState;
    uint8_t previousButtonState;
} Gamepad;

typedef struct Input
{
	uint32_t mouseX;
	uint32_t mouseY;
	MILK_BOOL mouseDown;
	MILK_BOOL mouseDownPrevious;
	Gamepad gamepad;
} Input;

/*
 * Code is the client side script(s).
 * - state holds the current state of the program. In this case, it is a Lua state.
 */
typedef struct Code
{
	void *state;
} Code;

typedef struct Milk
{
	Input input;
    Audio audio;
	Video video;
	Code code;
} Milk;

Milk *milkInit();
void milkFree(Milk *milk);

void milkUpdate(Milk *milk);
void milkDraw(Milk *milk);

int milkButton(Input *input, uint8_t button);
int milkButtonPressed(Input *input, uint8_t button);

#endif
