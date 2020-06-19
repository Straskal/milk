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

#ifndef __CONSOLE__H__
#define __CONSOLE__H__

#include "milk.h"

#include <stdbool.h>

#define COMMAND_MAX_LENGTH 25

typedef enum consoleInputState
{
    CONSOLE_INPUT_NONE =    0 << 0,
    CONSOLE_INPUT_CHAR =    1 << 0,
    CONSOLE_INPUT_BACK =    1 << 1,
    CONSOLE_INPUT_ENTER =   1 << 2,
    CONSOLE_INPUT_ESCAPE =  1 << 3
} ConsoleInputState;

typedef struct consoleInput
{
    void(*startTextInput)();
    void(*stopTextInput)();

    ConsoleInputState   state;
    ConsoleInputState   previousState;
    char                currentChar;
} ConsoleInput;

typedef enum consoleState
{
    GAME,
    COMMAND
} ConsoleState;

typedef struct console
{
    ConsoleInput    input;
    ConsoleState    state;
    int             lastErrorCount;
    int             commandCandidateLength;
    int             previousCommandLength;
    char            commandCandidate[COMMAND_MAX_LENGTH];
    char            previousCommand[COMMAND_MAX_LENGTH];
    bool            isGameInitialized;
} Console;

Console *createConsole();
void freeConsole(Console *console);
void updateConsole(Console *console, Milk *milk);
void drawConsole(Console *console, Milk *milk);

#endif
