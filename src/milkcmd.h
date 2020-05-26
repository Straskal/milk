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

#ifndef __MILK_COMMAND_LINE_H__
#define __MILK_COMMAND_LINE_H__

#include "milk.h"

#define MILK_COMMAND_LEN 25

typedef enum cmdState
{
	GAME,
	COMMAND
} CmdState;

typedef struct milkCmd
{
	CmdState state;
    size_t commandCandidateLength;
    size_t previousCommandLength;
    char commandCandidate[MILK_COMMAND_LEN];
    char previousCommand[MILK_COMMAND_LEN];
    int lastErrorCount;
} MilkCmd;

MilkCmd *milkCmdCreate();
void milkCmdFree(MilkCmd *cmd);
void milkCmdUpdate(MilkCmd *cmd, Milk *milk);
void milkCmdDraw(MilkCmd *cmd, Milk *milk);

#endif
