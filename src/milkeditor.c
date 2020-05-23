#include "milkeditor.h"
#include "milkapi.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static unsigned int _ticks = 0;

typedef struct Command
{
	char *cmd;
	void(*execute)(MilkEditor *, Milk *);
} Command;

static void _cmdReload(MilkEditor *editor, Milk *milk)
{
	if (editor->isGameInitialized)
		milkUnloadScripts(milk);

	milkLoadScripts(milk);
	editor->isGameInitialized = 1;
}

static Command _commands[] =
{
	{ "reload", _cmdReload }
};

static Command *_parseCommand(const char *cmd)
{
	for (int i = 0; i < 1; i++)
	{
		if (strcmp(_commands[i].cmd, cmd) == 0)
			return &_commands[i];
	}
	return NULL;
}

static void _resetCommandCandidate(CommandLine *cmdLine)
{
	cmdLine->commandCandidate[0] = '>';
	cmdLine->commandCandidate[1] = ':';
	cmdLine->commandCandidate[2] = '\0';
	cmdLine->commandCandidateLength = 2;
}

static void _updateCommandLine(MilkEditor *editor, Milk *milk)
{
	CommandLine *cmdLine = &editor->commandLine;
	char ch;

	if (milk->system.backspace() && cmdLine->commandCandidateLength > 2)
		cmdLine->commandCandidate[--cmdLine->commandCandidateLength] = '\0';

	if (milk->system.readTextInput(&ch))
	{
		cmdLine->commandCandidate[cmdLine->commandCandidateLength++] = ch;
		cmdLine->commandCandidate[cmdLine->commandCandidateLength] = '\0';
	}

	if (milk->system.enter())
	{
		Command *cmd = _parseCommand(&cmdLine->commandCandidate[2]);
		if (cmd != NULL)
			cmd->execute(editor, milk);		

		_resetCommandCandidate(cmdLine);
	}
}

static void _drawCommandLine(CommandLine *cmdLine, Milk *milk)
{
	size_t cmdLength = cmdLine->commandCandidateLength;

	milkClear(&milk->video, 0x1a1a1a);
	milkSpriteFont(&milk->video, 8, 10, "MILK\n------------------------------", 1);
	milkSpriteFont(&milk->video, 8, 40, cmdLine->commandCandidate, 1);

	/* Draw blinking position marker. */
	if (_ticks % 32 > 16 == 0)
		milkSpriteFont(&milk->video, (cmdLength + 1) * 8, 42, "_", 1);
}

MilkEditor *milkEditorInit()
{
	MilkEditor *editor = calloc(1, sizeof(MilkEditor));
	editor->state = COMMAND;
	editor->isGameInitialized = 0;
	_resetCommandCandidate(&editor->commandLine);
	return editor;
}

void milkEditorUpdate(MilkEditor *editor, Milk *milk)
{
	milkResetDrawState(&milk->video);

	if (milk->system.escape() && editor->isGameInitialized)
	{
		if (editor->state != COMMAND)
		{
			editor->state = COMMAND;
			milk->system.startTextInput();
		}
		else
		{
			editor->state = GAME;
			milk->system.stopTextInput();
		}
	}

	switch (editor->state)
	{
		case COMMAND:
			_updateCommandLine(editor, milk);
			break;
		case GAME:
			milkResetDrawState(&milk->video);
			milkInvokeUpdate(&milk->code);
			break;
	}
}

void milkEditorDraw(MilkEditor *editor, Milk *milk)
{
	switch (editor->state)
	{
	case COMMAND:
		_drawCommandLine(&editor->commandLine, milk);
		break;
	case GAME:
		milkInvokeDraw(&milk->code);
		break;
	}
	_ticks++;
}

void milkEditorFree(MilkEditor *editor)
{
	free(editor);
}
