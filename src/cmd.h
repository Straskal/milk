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

#ifndef __MILK_COMMAND_LINE_H__
#define __MILK_COMMAND_LINE_H__

#include "milk.h"

#include <stdbool.h>

#define MILK_COMMAND_LEN 25


/*
 *******************************************************************************
 * System
 *
 * Platform code is responsible for providing system implementation.
 *******************************************************************************
 */


typedef enum cmdInputState
{
	INPUT_NONE =    0u << 0u,
    INPUT_CHAR =    1u << 0u,
    INPUT_BACK =    1u << 1u,
    INPUT_ENTER =   1u << 2u,
    INPUT_ESCAPE =  1u << 3u
} CmdInputState;


typedef struct cmdInput
{
    void(*startTextInput)();
    void(*stopTextInput)();
    CmdInputState state;
    char currentChar;
} CmdInput;


/*
 *******************************************************************************
 * Command line
 *******************************************************************************
 */


typedef enum cmdState
{
	GAME,
	COMMAND
} CmdState;


typedef struct milkCmd
{
    CmdInput    input;
	CmdState    state;
    int         lastErrorCount;
    int         commandCandidateLength;
    int         previousCommandLength;
    char        commandCandidate[MILK_COMMAND_LEN];
    char        previousCommand[MILK_COMMAND_LEN];
    bool        isGameInitialized;
} MilkCmd;


MilkCmd *createCmd();
void freeCmd(MilkCmd *cmd);
void updateCmd(MilkCmd *cmd, Milk *milk);
void drawCmd(MilkCmd *cmd, Milk *milk);

#endif
