/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include <stdbool.h>

typedef enum buttonState
{
    BTN_NONE    = 0 << 0,
    BTN_START   = 1 << 0,
    BTN_UP      = 1 << 1,
    BTN_DOWN    = 1 << 2,
    BTN_LEFT    = 1 << 3,
    BTN_RIGHT   = 1 << 4,
    BTN_A       = 1 << 5,
    BTN_B       = 1 << 6,
    BTN_X       = 1 << 7,
    BTN_Y       = 1 << 8,
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

bool isButtonDown(Input *input, ButtonState button);

bool isButtonPressed(Input *input, ButtonState button);

#endif
