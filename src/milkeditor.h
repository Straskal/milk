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

#ifndef __MILK_EDITOR_H__
#define __MILK_EDITOR_H__

#include "milk.h"

#define COMMAND_QUEUE_MAX 25
#define COMMAND_LENGTH 25

typedef enum
{
	GAME,
	COMMAND
} EditorState;

typedef struct CommandQueue
{
	char *command;
	struct CommandQueue *next;
} CommandQueue;

typedef struct CommandLine
{
	CommandQueue commandQueue;
	size_t commandQueueLength;
	size_t commandCandidateLength;
	char commandCandidate[COMMAND_LENGTH];
} CommandLine;

typedef struct
{
	EditorState state;
	CommandLine commandLine;
	uint8_t isGameInitialized;
} MilkEditor;

MilkEditor *milkEditorInit();
void milkEditorUpdate(MilkEditor *editor, Milk *milk);
void milkEditorDraw(MilkEditor *editor, Milk *milk);
void milkEditorFree(MilkEditor *editor);

#endif
