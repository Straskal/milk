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
