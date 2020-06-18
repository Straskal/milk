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

#include "api.h"
#include "cmd.h"
#include "embed/font.h"

#include <string.h>

#define CMD_MAX_ARGS	8
#define CMD_DELIM		" "

#define CMD_COLOR		0xffffff
#define CMD_COLOR_INFO	0x5c5c5c
#define CMD_COLOR_ERROR 0xbf4040
#define CMD_COLOR_WARN	0xffec27

#define LOG_START_HEIGHT 56
#define LOG_END_HEIGHT	(224 - 32)
#define MAX_LINES		((LOG_END_HEIGHT - LOG_START_HEIGHT) / CHAR_WIDTH)
#define CHARS_PER_LINE	31

#define HAS_INPUT(inputState, input) ((inputState & input) == input)


static unsigned int ticks = 0;


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


static void cmdReload(MilkCmd *cmd, Milk *milk, char *args[], int nargs)
{
	(void)cmd;
	(void)args;
	(void)nargs;
	milkUnloadCode(milk);
	milkLoadCode(milk);
	cmd->isGameInitialized = false;
	logMessage(&milk->logs, "Scripts have been reloaded", INFO);
}


static void cmdClear(MilkCmd *cmd, Milk *milk, char *args[], int nargs)
{
	(void)cmd;
	(void)args;
	(void)nargs;
	clearLogs(&milk->logs);
	cmd->lastErrorCount = 0;
}


static void cmdQuit(MilkCmd *cmd, Milk *milk, char *args[], int nargs)
{
	(void)cmd;
	(void)args;
	(void)nargs;
	milk->shouldQuit = true;
}


static CommandImpl _commands[] =
{
	{ "reload",	cmdReload },
	{ "clear",	cmdClear },
	{ "quit",	cmdQuit }
};

#define NUM_COMMANDS sizeof(_commands) / sizeof(CommandImpl)


/*
 *******************************************************************************
 * Command line
 *******************************************************************************
 */


static CommandImpl *findCommand(const char *cmd)
{
	for (size_t i = 0; i < NUM_COMMANDS; i++)
	{
		if (strcmp(cmd, _commands[i].cmd) == 0)
			return &_commands[i];
	}
	return NULL;
}


static CommandImpl *parseCommand(char *cmd, char *args[], int *nargs)
{
	*nargs = 0;
	char tempCmd[MILK_COMMAND_LEN];
	strcpy(tempCmd, cmd);
	char *token = strtok(tempCmd, CMD_DELIM); /* Get command name. */
	CommandImpl *command = findCommand(token);

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


static void resetCommandCandidate(MilkCmd *cmdLine)
{
	cmdLine->commandCandidateLength = 0;

	for (int i = 0; i < MILK_COMMAND_LEN; i++)
		cmdLine->commandCandidate[i] = 0;
}


static void updateCommandLine(MilkCmd *cmd, Milk *milk)
{
	CmdInputState btnState = cmd->input.state;

	if (HAS_INPUT(btnState, INPUT_BACK) && cmd->commandCandidateLength > 0)
		cmd->commandCandidate[--cmd->commandCandidateLength] = '\0';

	if (HAS_INPUT(btnState, INPUT_CHAR) && cmd->commandCandidateLength < MILK_COMMAND_LEN - 1)
	{
		cmd->commandCandidate[cmd->commandCandidateLength++] = cmd->input.currentChar;
		cmd->commandCandidate[cmd->commandCandidateLength] = '\0';
	}

	if (isButtonPressed(&milk->input, BTN_UP) && cmd->previousCommandLength > 0)
	{
		strcpy(cmd->commandCandidate, cmd->previousCommand);
		cmd->commandCandidateLength = cmd->previousCommandLength;
	}

	if (isButtonPressed(&milk->input, BTN_DOWN))
		resetCommandCandidate(cmd);

	if (HAS_INPUT(btnState, INPUT_ENTER) && cmd->commandCandidateLength > 0)
	{
		char *args[CMD_MAX_ARGS];
		int nargs;

		CommandImpl *cmdImpl = parseCommand(cmd->commandCandidate, args, &nargs);
		if (cmdImpl != NULL)
		{
			cmdImpl->execute(cmd, milk, args, nargs);

			/* After executing valid command, update the previous command. */
			strcpy(cmd->previousCommand, cmd->commandCandidate);
			cmd->previousCommandLength = cmd->commandCandidateLength;
		}
		else logMessage(&milk->logs, "Unknown command", WARN);

		resetCommandCandidate(cmd);
	}
}


static Color32 getLogColor(LogType type)
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
static void getLogLines(Logs *logs, CommandLogLine *lines, int *numLines)
{
	int currentLine = 0;

	for (int i = logs->count - 1; i >= 0; i--)
	{
		if (currentLine == MAX_LINES - 1)
			break;

		char tempText[MAX_LOG_LENGTH + 3] = ">:";
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
				lines[currentLine].color = getLogColor(logs->messages[i].type);
				messageLength -= lineLength;
				messageText += lineLength;
				currentLine++;
			}

			splitByNewline = strtok(NULL, "\n");
		}
	}

	*numLines = currentLine;
}


static void drawLogLines(Milk *milk)
{
	CommandLogLine lines[MAX_LINES];
	int numLines;

	getLogLines(&milk->logs, lines, &numLines);

	for (int i = 0; i < numLines; i++)
		blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 8, LOG_START_HEIGHT + ((CHAR_HEIGHT + 2) * i),
		               lines[i].text, 1, lines[i].color);
}


static void drawCommandLine(MilkCmd *cmdLine, Milk *milk)
{
	int cmdLength = cmdLine->commandCandidateLength;

	clearFramebuffer(&milk->video, 0x000000);
	blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 8, 10, "MILK [1.0.0]\n------------------------------", 1, CMD_COLOR);
	blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 8, 40, ">:", 1, CMD_COLOR);
	blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 24, 40, cmdLine->commandCandidate, 1, CMD_COLOR);

	/* Draw blinking position marker. */
    if (ticks % 64 < 48)
        blitFilledRectangle(&milk->video, 24 + cmdLength * CHAR_WIDTH, 40, 8, 8, 0xc10a31);
}


MilkCmd *createCmd()
{
	MilkCmd *cmd = calloc(1, sizeof(MilkCmd));
	cmd->state = COMMAND;
	resetCommandCandidate(cmd);
	return cmd;
}


void freeCmd(MilkCmd *cmd)
{
	free(cmd);
}


static void errorCheck(MilkCmd *cmd, Milk *milk)
{
	if (cmd->lastErrorCount < milk->logs.errorCount)
	{
		cmd->lastErrorCount = milk->logs.errorCount;
		cmd->state = COMMAND;
		cmd->input.startTextInput();
	}
}


void updateCmd(MilkCmd *cmd, Milk *milk)
{
	CmdInputState btnState = cmd->input.state;
	resetDrawState(&milk->video);

	if (HAS_INPUT(btnState, INPUT_ESCAPE))
	{
		if (cmd->state != COMMAND)
		{
			cmd->state = COMMAND;
			cmd->input.startTextInput();
		}
		else
		{
			if (!cmd->isGameInitialized)
			{
				milkInvokeInit(&milk->code);
				cmd->isGameInitialized = true;
			}
			cmd->state = GAME;
			cmd->input.stopTextInput();
		}
	}

	switch (cmd->state)
	{
		case COMMAND:
			updateCommandLine(cmd, milk);
			break;
		case GAME:
			milkInvokeUpdate(&milk->code);
			errorCheck(cmd, milk);
			break;
	}
}


void drawCmd(MilkCmd *cmd, Milk *milk)
{
	switch (cmd->state)
	{
		case COMMAND:
			drawCommandLine(cmd, milk);
			drawLogLines(milk);
			break;
		case GAME:
			milkInvokeDraw(&milk->code);
			errorCheck(cmd, milk);
			break;
	}
	ticks++;
}
