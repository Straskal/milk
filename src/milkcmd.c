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

#include "milkapi.h"
#include "milkcmd.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define CMD_MAX_ARGS 8
#define CMD_DELIM " "

#define CMD_RELOAD "reload"
#define CMD_RELOAD_SCRIPTS "scripts"
#define CMD_RELOAD_SPRITES "sprites"
#define CMD_RELOAD_FONT "font"
#define CMD_CLEAR "clear"
#define CMD_QUIT "quit"

#define CMD_COLOR 0xffffff
#define CMD_COLOR_INFO 0x5c5c5c
#define CMD_COLOR_ERROR 0xbf4040
#define CMD_COLOR_WARN 0xffec27

#define LOG_START_HEIGHT 56
#define LOG_END_HEIGHT (224 - 32)
#define MAX_LINES ((LOG_END_HEIGHT - LOG_START_HEIGHT) / MILK_CHAR_SQRSIZE)
#define CHARS_PER_LINE 31

static unsigned int _ticks = 0;

typedef struct commandImpl
{
	char *cmd;
	void(*execute)(MilkCmd *, Milk *, char **, int);
} CommandImpl;

typedef struct commandLogLine
{
	char text[CHARS_PER_LINE];
	Color32 color;
} CommandLogLine;

/*
 *******************************************************************************
 * Commands
 *******************************************************************************
 */

static void _cmdReload(MilkCmd *cmd, Milk *milk, char *args[], int nargs)
{
	(void *)cmd;

	if (nargs == 1)
	{
		if (strcmp(args[0], CMD_RELOAD_SCRIPTS) == 0)
		{
			milkUnloadCode(milk);
			milkLoadCode(milk);
			milkLog(milk, "Scripts have been reloaded", INFO);
		}
		else if (strcmp(args[0], CMD_RELOAD_SPRITES) == 0)
		{
			milkLoadSpritesheet(&milk->video);
			milkLog(milk, "Sprites have been reloaded", INFO);
		}
		else if (strcmp(args[0], CMD_RELOAD_FONT) == 0)
		{
			milkLoadFont(&milk->video);
			milkLog(milk, "Font has been reloaded", INFO);
		}
	}
	else milkLog(milk, "'reload' expects an argument", WARN);
}

static void _cmdClear(MilkCmd *cmd, Milk *milk, char *args[], int nargs)
{
	(void *)cmd;
	(void *)args;
	(void *)nargs;

	milkClearLogs(milk);
	cmd->lastErrorCount = 0;
}

static void _cmdQuit(MilkCmd *cmd, Milk *milk, char *args[], int nargs)
{
	(void *)cmd;
	(void *)args;
	(void *)nargs;

	milk->shouldQuit = 1;
}

static CommandImpl _commands[] =
{
	{ CMD_RELOAD, _cmdReload },
	{ CMD_CLEAR, _cmdClear },
	{ CMD_QUIT, _cmdQuit }
};

#define NUM_COMMANDS sizeof(_commands) / sizeof(CommandImpl)

/*
 *******************************************************************************
 * Command line
 *******************************************************************************
 */

static CommandImpl *_findCommand(const char *cmd)
{
	for (size_t i = 0; i < NUM_COMMANDS; i++)
	{
		if (strcmp(cmd, _commands[i].cmd) == 0)
			return &_commands[i];
	}
	return NULL;
}

static CommandImpl *_parseCommand(char *cmd, char *args[], int *nargs)
{
	*nargs = 0;
	char tempCmd[MILK_COMMAND_LEN];
	strcpy(tempCmd, cmd);
	char *token = strtok(tempCmd, CMD_DELIM); /* Get command name. */
	CommandImpl *command = _findCommand(token);

	if (command != NULL) /* Parse args. */
	{
		token = strtok(NULL, CMD_DELIM);
		while (token != NULL)
		{
			args[*nargs] = token;
			token = strtok(NULL, CMD_DELIM);
			(*nargs)++;
		}
	}
	return command;
}

static void _resetCommandCandidate(MilkCmd *cmdLine)
{
	cmdLine->commandCandidateLength = 0;

	for (int i = 0; i < MILK_COMMAND_LEN; i++)
		cmdLine->commandCandidate[i] = 0;
}

static void _updateCommandLine(MilkCmd *cmd, Milk *milk)
{
	char ch;

	if (cmd->system.backspace() && cmd->commandCandidateLength > 0)
		cmd->commandCandidate[--cmd->commandCandidateLength] = '\0';

	if (cmd->system.readTextInput(&ch) && cmd->commandCandidateLength < MILK_COMMAND_LEN - 1)
	{
		cmd->commandCandidate[cmd->commandCandidateLength++] = ch;
		cmd->commandCandidate[cmd->commandCandidateLength] = '\0';
	}

	if (milkButtonPressed(&milk->input, BTN_UP) && cmd->previousCommandLength > 0)
	{
		strcpy(cmd->commandCandidate, cmd->previousCommand);
		cmd->commandCandidateLength = cmd->previousCommandLength;
	}

	if (milkButtonPressed(&milk->input, BTN_DOWN))
		_resetCommandCandidate(cmd);

	if (cmd->system.enter() && cmd->commandCandidateLength > 0)
	{
		char *args[CMD_MAX_ARGS];
		int nargs;

		CommandImpl *cmdImpl = _parseCommand(cmd->commandCandidate, args, &nargs);
		if (cmdImpl != NULL)
		{
			cmdImpl->execute(cmd, milk, args, nargs);

			/* After executing valid command, update the previous command. */
			strcpy(cmd->previousCommand, cmd->commandCandidate);
			cmd->previousCommandLength = cmd->commandCandidateLength;
		}
		else milkLog(milk, "Unknown command", WARN);

		_resetCommandCandidate(cmd);
	}
}

static Color32 _getLogColor(LogType type)
{
	switch (type)
	{
		case INFO:
			return CMD_COLOR_INFO;
		case WARN:
			return CMD_COLOR_WARN;
		case ERROR:
			return CMD_COLOR_ERROR;
		default:
			return CMD_COLOR_INFO;
	}
}

/* This is poopy. Need to refactor. */
static void _getLogLines(Logs *logs, CommandLogLine *lines, int *numLines)
{
	int currentLine = 0;

	for (int i = logs->count - 1; i >= 0; i--)
	{
		if (currentLine == MAX_LINES - 1)
			break;

		char tempText[MILK_LOG_MAX_LENGTH + 3] = ">:";

		strcpy(&tempText[2], logs->messages[i].text);
		char *splitByNewline = strtok(tempText, "\n"); /* Split message by newline. */

		while (splitByNewline != NULL && currentLine < MAX_LINES - 1)
		{
			size_t messageLength = strlen(splitByNewline);
			char *messageText = splitByNewline;

			while (messageLength > 0 && currentLine < MAX_LINES - 1) /* Draw the message in separate line. */
			{
				size_t remainingLength = strlen(messageText);
				size_t lineLength = remainingLength > CHARS_PER_LINE - 1 ? CHARS_PER_LINE - 1 : remainingLength;
				strncpy(lines[currentLine].text, messageText, lineLength);
				lines[currentLine].text[lineLength] = '\0';
				lines[currentLine].color = _getLogColor(logs->messages[i].type);
				messageLength -= lineLength;
				messageText += lineLength;
				currentLine++;
			}

			splitByNewline = strtok(NULL, "\n");
		}
	}

	*numLines = currentLine;
}

static void _drawLogLines(Milk *milk)
{
	CommandLogLine lines[MAX_LINES];
	int numLines;

	_getLogLines(&milk->logs, lines, &numLines);

	for (int i = 0; i < numLines; i++)
		milkSpriteFont(&milk->video, 8, LOG_START_HEIGHT + ((MILK_CHAR_SQRSIZE + 2) * i), lines[i].text, 1, lines[i].color);
}

static void _drawCommandLine(MilkCmd *cmdLine, Milk *milk)
{
	size_t cmdLength = cmdLine->commandCandidateLength;

	milkClear(&milk->video, 0x1a1a1a);
	milkSpriteFont(&milk->video, 8, 10, "MILK\n------------------------------", 1, CMD_COLOR);
	milkSpriteFont(&milk->video, 8, 40, ">:", 1, CMD_COLOR);
	milkSpriteFont(&milk->video, 24, 40, cmdLine->commandCandidate, 1, CMD_COLOR);

	/* Draw blinking position marker. */
	if (_ticks % 32 > 16)
		milkSpriteFont(&milk->video, 24 + cmdLength * 8, 42, "_", 1, CMD_COLOR);
}

MilkCmd *milkCmdCreate()
{
	MilkCmd *cmd = calloc(1, sizeof(MilkCmd));
	cmd->state = COMMAND;
	_resetCommandCandidate(cmd);
	return cmd;
}

void milkCmdFree(MilkCmd *cmd)
{
	free(cmd);
}

static void _errorCheck(MilkCmd *cmd, Milk *milk)
{
	if (cmd->lastErrorCount < milk->logs.errorCount)
	{
		cmd->lastErrorCount = milk->logs.errorCount;
		cmd->state = COMMAND;
		cmd->system.startTextInput();
	}
}

void milkCmdUpdate(MilkCmd *cmd, Milk *milk)
{
	milkResetDrawState(&milk->video);

	if (cmd->system.escape())
	{
		if (cmd->state != COMMAND)
		{
			cmd->state = COMMAND;
			cmd->system.startTextInput();
		}
		else
		{
			cmd->state = GAME;
			cmd->system.stopTextInput();
		}
	}

	switch (cmd->state)
	{
		case COMMAND:
			_updateCommandLine(cmd, milk);
			break;
		case GAME:
			milkInvokeUpdate(&milk->code);
			_errorCheck(cmd, milk);
			break;
	}
}

void milkCmdDraw(MilkCmd *cmd, Milk *milk)
{
	switch (cmd->state)
	{
		case COMMAND:
			_drawCommandLine(cmd, milk);
			_drawLogLines(milk);
			break;
		case GAME:
			milkInvokeDraw(&milk->code);
			_errorCheck(cmd, milk);
			break;
	}
	_ticks++;
}
