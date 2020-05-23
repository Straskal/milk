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

#include "milkeditor.h"
#include "milkapi.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_DELIM " "
#define CMD_RELOAD "reload"
#define CMD_RELOAD_SCRIPTS "scripts"
#define CMD_RELOAD_SPRITES "sprites"
#define CMD_RELOAD_FONT "font"
#define CMD_CLEAR "clear"

static unsigned int _ticks = 0;

/*
 *******************************************************************************
 * COMMAND LINE
 *******************************************************************************
 */

static void _cmdReload(MilkEditor *editor, Milk *milk, char *args[], int nargs)
{
	(void *)editor;

	if (nargs == 1)
	{
		if (strcmp(args[0], CMD_RELOAD_SCRIPTS) == 0)
		{
			milkUnloadScripts(milk);
			milkLoadScripts(milk);
			milkLog(milk, "Scripts have been reloaded", INFO);
		}
		else if (strcmp(args[0], CMD_RELOAD_SPRITES) == 0)
		{
			milkLoadBmp("sprsheet.bmp", milk->video.spritesheet, MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SQRSIZE);
			milkLog(milk, "Sprites have been reloaded", INFO);
		}
		else if (strcmp(args[0], CMD_RELOAD_FONT) == 0)
		{
			milkLoadBmp(MILK_FONT_FILENAME, milk->video.font, MILK_FONT_WIDTH * MILK_FONT_HEIGHT);
			milkLog(milk, "Font has been reloaded", INFO);
		}
	}
	else milkLog(milk, "reload command expects an argument", WARN);
}

static void _cmdClear(MilkEditor *editor, Milk *milk, char *args[], int nargs)
{
	(void *)editor;
	(void *)args;
	(void *)nargs;
	milkClearLogs(milk);
}

typedef struct CommandImpl
{
	char *cmd;
	void(*execute)(MilkEditor *, Milk *, char **, int);
} CommandImpl;

static CommandImpl _commands[] =
{
	{ CMD_RELOAD, _cmdReload },
	{ CMD_CLEAR, _cmdClear }
};

static CommandImpl *_findCommand(const char *cmd)
{
	size_t len = sizeof(_commands) / sizeof(CommandImpl);
	for (int i = 0; i < len; i++)
	{
		if (strcmp(cmd, _commands[i].cmd) == 0)
			return &_commands[i];
	}
	return NULL;
}

static CommandImpl *_parseCommand(char *cmd, char *args[], int *nargs)
{
	*nargs = 0;
	char tempCmd[COMMAND_LENGTH];
	strcpy(tempCmd, cmd);
	char *token = strtok(tempCmd, COMMAND_DELIM); /* Get command name. */
	CommandImpl *command = _findCommand(token);

	if (command != NULL) /* Parse args. */
	{
		token = strtok(NULL, COMMAND_DELIM);
		while (token != NULL)
		{
			args[*nargs] = token;
			token = strtok(NULL, COMMAND_DELIM);
			(*nargs)++;
		}
	}
	return command;
}

static void _resetCommandCandidate(CommandLine *cmdLine)
{
	cmdLine->commandCandidateLength = 0;

	for (int i = 0; i < COMMAND_LENGTH; i++)
		cmdLine->commandCandidate[i] = 0;
}

static void _updateCommandLine(MilkEditor *editor, Milk *milk)
{
	CommandLine *cmdLine = &editor->commandLine;
	char ch;

	if (milk->system.backspace() && cmdLine->commandCandidateLength > 0)
		cmdLine->commandCandidate[--cmdLine->commandCandidateLength] = 0;

	if (milk->system.readTextInput(&ch) && cmdLine->commandCandidateLength < COMMAND_LENGTH - 1)
	{
		cmdLine->commandCandidate[cmdLine->commandCandidateLength++] = ch;
		cmdLine->commandCandidate[cmdLine->commandCandidateLength] = 0;
	}

	if (milkButtonPressed(&milk->input, BTN_UP) && cmdLine->previousCommandLength > 0)
	{
		strcpy(cmdLine->commandCandidate, cmdLine->previousCommand);
		cmdLine->commandCandidateLength = cmdLine->previousCommandLength;
	}

	if (milkButtonPressed(&milk->input, (1 << 1)))
		_resetCommandCandidate(cmdLine);

	if (milk->system.enter() && cmdLine->commandCandidateLength > 0)
	{
		char *args[COMMAND_MAX_ARGS];
		int nargs;

		CommandImpl *cmd = _parseCommand(cmdLine->commandCandidate, args, &nargs);
		if (cmd != NULL)
		{
			cmd->execute(editor, milk, args, nargs);

			/* After executing valid command, update the previous command. */
			strcpy(cmdLine->previousCommand, cmdLine->commandCandidate);
			cmdLine->previousCommandLength = cmdLine->commandCandidateLength;
		}
		else milkLog(milk, "Unknown command", INFO);

		_resetCommandCandidate(cmdLine);
	}
}

static void _drawCommandLine(CommandLine *cmdLine, Milk *milk)
{
	size_t cmdLength = cmdLine->commandCandidateLength;
	int logYPosition = 56;

	milkClear(&milk->video, 0x1a1a1a);
	milkSpriteFont(&milk->video, 8, 10, "MILK\n------------------------------", 1);
	milkSpriteFont(&milk->video, 8, 40, ">:", 1);
	milkSpriteFont(&milk->video, 24, 40, cmdLine->commandCandidate, 1);

	/* Draw blinking position marker. */
	if (_ticks % 32 > 16)
		milkSpriteFont(&milk->video, 24 + cmdLength * 8, 42, "_", 1);

	for (int i = 0; i < milk->logs.count; i++)
	{
		milkSpriteFont(&milk->video, 8, logYPosition, milk->logs.logs[i].message, 1);
		logYPosition += 9;
	}
}

MilkEditor *milkEditorInit()
{
	MilkEditor *editor = calloc(1, sizeof(MilkEditor));
	editor->state = COMMAND;
	_resetCommandCandidate(&editor->commandLine);
	return editor;
}

void milkEditorUpdate(MilkEditor *editor, Milk *milk)
{
	milkResetDrawState(&milk->video);

	if (milk->system.escape())
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
