#include "common.h"
#include "console.h"
#include "embed/font.h"
#include "logs.h"

#include <string.h>

// Track number of ticks since engine has started.
static unsigned int ticks = 0;

// Unloads the current scripts.
static void cmdUnload(Console *console, Milk *milk, char **arguments, int argumentCount)
{
	UNUSED(console);
	UNUSED(arguments);
	UNUSED(argumentCount);

	unloadCode(milk);
	console->isGameInitialized = false;

	LOG_INFO("scripts have been unloaded");
}

// Clears the logs.
static void cmdClear(Console *console, Milk *milk, char **arguments, int argumentCount)
{
	UNUSED(console);
	UNUSED(milk);
	UNUSED(arguments);
	UNUSED(argumentCount);

	LOG_CLEAR();
	console->lastErrorCount = 0;
}

// Quits the engine.
static void cmdQuit(Console *console, Milk *milk, char **arguments, int argumentCount)
{
	UNUSED(console);
	UNUSED(arguments);
	UNUSED(argumentCount);

	milk->shouldQuit = true;
}

typedef struct command
{
	char *cmd;
	void(*execute)(Console *, Milk *, char **, int);
} Command;

static Command commands[] =
{
		{ "unload", cmdUnload },
    { "-u",     cmdUnload },
		{ "clear",  cmdClear },
    { "-c",     cmdClear },
		{ "quit",   cmdQuit },
    { "-q",     cmdQuit }
};

#define NUM_COMMANDS sizeof(commands) / sizeof(Command)

static void resetCommandCandidate(Console *console)
{
	console->commandCandidateLength = 0;
	for (int i = 0; i < COMMAND_MAX_LENGTH; i++)
		console->commandCandidate[i] = '\0';
}

Console *createConsole()
{
	Console *console = calloc(1, sizeof(Console));
	console->state = COMMAND;
	resetCommandCandidate(console);
	return console;
}

void freeConsole(Console *console)
{
	free(console);
}

#define MAX_COMMAND_ARGUMENTS	8
#define COMMAND_DELIMITER	" "

static char *getCommandName(char *candidate)
{
	return strtok(candidate, COMMAND_DELIMITER);
}

static Command *getCommand(const char *commandName)
{
	for (size_t i = 0; i < NUM_COMMANDS; i++)
	{
		if (strcmp(commandName, commands[i].cmd) == 0)
			return &commands[i];
	}
	return NULL;
}

static void getCommandParameters(const char *candidateArguments, char **arguments, int *argumentCount)
{
	candidateArguments = strtok(NULL, COMMAND_DELIMITER);
	while (candidateArguments != NULL && *argumentCount < MAX_COMMAND_ARGUMENTS)
	{
		strcpy(arguments[(*argumentCount)++], candidateArguments);
		candidateArguments = strtok(NULL, COMMAND_DELIMITER);
	}
}

/* <command> <...args> */
static Command *parseCommand(char *candidate, char **arguments, int *argumentCount)
{
	*argumentCount = 0;
	char candidateCopy[COMMAND_MAX_LENGTH];
	strcpy(candidateCopy, candidate);
	char *currentToken = getCommandName(candidateCopy);
	Command *command = getCommand(currentToken);

	if (command != NULL)
		getCommandParameters(currentToken, arguments, argumentCount);

	return command;
}

static bool hasInputContinuous(ConsoleInput *input, ConsoleInputState inputState)
{
	return IS_BIT_SET(input->state, inputState);
}

static bool hasInput(ConsoleInput *input, ConsoleInputState inputState)
{
	ConsoleInputState currentState = input->state;
	ConsoleInputState previousState = input->previousState;
	return IS_BIT_SET(currentState, inputState) && !IS_BIT_SET(previousState, inputState);
}

static void handleBackspace(Console *console)
{
	if (hasInputContinuous(&console->input, CONSOLE_INPUT_BACK) && console->commandCandidateLength > 0)
		console->commandCandidate[--console->commandCandidateLength] = '\0';
}

static void handleCharacterInput(Console *console)
{
	if (hasInputContinuous(&console->input, CONSOLE_INPUT_CHAR) && console->commandCandidateLength < COMMAND_MAX_LENGTH - 1)
	{
		console->commandCandidate[console->commandCandidateLength] = console->input.currentChar;
		console->commandCandidate[++console->commandCandidateLength] = '\0';
	}
}

static void handleGoToPreviousCommand(Console *console, Milk *milk)
{
	if (isButtonPressed(&milk->input, BTN_UP) && console->previousCommandLength > 0)
	{
		strcpy(console->commandCandidate, console->previousCommand);
		console->commandCandidateLength = console->previousCommandLength;
	}
}

static void handleClearCandidate(Console *console, Milk *milk)
{
	if (isButtonPressed(&milk->input, BTN_DOWN))
		resetCommandCandidate(console);
}

static void handleEnter(Console *console, Milk *milk)
{
	if (hasInput(&console->input, CONSOLE_INPUT_ENTER) && console->commandCandidateLength > 0)
	{
		Command *command;
    int argumentCount;
		char *arguments[MAX_COMMAND_ARGUMENTS];

		for (int i = 0; i < MAX_COMMAND_ARGUMENTS; i++)
			arguments[i] = calloc(COMMAND_MAX_LENGTH, sizeof(char));

		if ((command = parseCommand(console->commandCandidate, arguments, &argumentCount)) != NULL)
		{
			command->execute(console, milk, arguments, argumentCount);
			strcpy(console->previousCommand, console->commandCandidate);
			console->previousCommandLength = console->commandCandidateLength;

			for (int i = 0; i < MAX_COMMAND_ARGUMENTS; i++)
				free(arguments[i]);
		}
		else LOG_WARN("Unknown command");

		resetCommandCandidate(console);
	}
}

static void handleEscape(Console *console, Milk *milk)
{
	if (hasInput(&console->input, CONSOLE_INPUT_ESCAPE))
	{
		if (console->state != COMMAND)
		{
			pauseSound(&milk->audio, -1);
			pauseStream(&milk->audio, -1);
			console->state = COMMAND;
			console->input.startTextInput();
		}
		else
		{
			if (!console->isGameInitialized)
			{
				loadCode(milk);
				invokeInit(&milk->code);
				console->isGameInitialized = true;
			}

			resumeSound(&milk->audio, -1);
      resumeStream(&milk->audio, -1);
			console->state = GAME;
			console->input.stopTextInput();
		}
	}
}

static void handleInput(Console *console, Milk *milk)
{
	handleBackspace(console);
	handleCharacterInput(console);
	handleGoToPreviousCommand(console, milk);
	handleClearCandidate(console, milk);
	handleEnter(console, milk);
}

static void haltOnError(Console *console)
{
	if (console->lastErrorCount < LOG_GET()->errorCount)
	{
		console->lastErrorCount = LOG_GET()->errorCount;
		console->state = COMMAND;
		console->input.startTextInput();
	}
}

void updateConsole(Console *console, Milk *milk)
{
	handleEscape(console, milk);

	switch (console->state)
	{
    case COMMAND:
      handleInput(console, milk);
      break;
    case GAME:
      invokeUpdate(&milk->code);
      haltOnError(console);
      break;
    default:
      break;
	}
}

#define VERSION_HEADER "MILK [1.0.0]"

#define COMMAND_TEXT_COLOR	0xffffff
#define COMMAND_INFO_COLOR  0x5c5c5c
#define COMMAND_ERROR_COLOR 0xbf4040
#define COMMAND_WARN_COLOR	0xffec27

#define LOG_START_HEIGHT 56
#define LOG_END_HEIGHT (224 - 32)
#define MAX_LINES	((LOG_END_HEIGHT - LOG_START_HEIGHT) / CHAR_WIDTH)
#define CHARS_PER_LINE 31

static void drawCommandLine(Console *console, Milk *milk)
{
	clearFramebuffer(&milk->video, 0x000000);
	blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 8, 10, VERSION_HEADER, 1, COMMAND_TEXT_COLOR);
	blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 8, 10 + CHAR_HEIGHT, "------------------------------", 1, COMMAND_TEXT_COLOR);
	blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 8, 40, ">:", 1, COMMAND_TEXT_COLOR);
	blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 24, 40, console->commandCandidate, 1, COMMAND_TEXT_COLOR);

	// Draw blinking position marker.
	if (ticks % 64 < 48)
		blitFilledRectangle(&milk->video, 24 + console->commandCandidateLength * CHAR_WIDTH, 40, 6, 8, 0xc10a31);
}

static void drawPlayingIndicator(Console *console, Milk *milk)
{
	const int size = 4;
	const int padding = 8;
	const int offset = size + padding;

	if (console->isGameInitialized)
	{
		blitFilledRectangle(&milk->video, FRAMEBUFFER_WIDTH - offset, offset, size, size, 0x00ff00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - offset, offset, 0x00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - padding - 1, offset, 0x00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - offset, offset + 3, 0x00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - padding - 1, offset + 3, 0x00);
	}
}

typedef struct consoleLine
{
	char text[CHARS_PER_LINE];
	Color32 color;
} ConsoleLine;

static Color32 getLogColor(LogType type)
{
	switch (type)
	{
    case INFO:
      return COMMAND_INFO_COLOR;
    case WARN:
      return COMMAND_WARN_COLOR;
    case ERROR:
      return COMMAND_ERROR_COLOR;
    default:
      return COMMAND_INFO_COLOR;
	}
}

static void getLogLines(Logs *logs, ConsoleLine *lines, int *numLines)
{
	int currentLine = 0;

	for (int i = logs->count - 1; i >= 0; i--)
	{
		if (currentLine == MAX_LINES - 1)
			break;

		char tempText[MAX_LOG_LENGTH + 3] = ">:";
		strcpy(&tempText[2], logs->messages[i].text);
		char *splitByNewline = strtok(tempText, "\n"); // Split message by newline.

		while (splitByNewline != NULL && currentLine < MAX_LINES - 1)
		{
			size_t messageLength = strlen(splitByNewline);
			char *messageText = splitByNewline;

			while (messageLength > 0 && currentLine < MAX_LINES - 1) // Draw the message in separate line.
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
	ConsoleLine lines[MAX_LINES];
	int numLines;

	getLogLines(LOG_GET(), lines, &numLines);

	for (int i = 0; i < numLines; i++)
		blitSpriteFont(&milk->video, DEFAULT_FONT_DATA, 8, LOG_START_HEIGHT + ((CHAR_HEIGHT + 2) * i), lines[i].text, 1, lines[i].color);
}

void drawConsole(Console *console, Milk *milk)
{
	switch (console->state)
	{
    case COMMAND:
      drawCommandLine(console, milk);
      drawPlayingIndicator(console, milk);
      drawLogLines(milk);
      break;
    case GAME:
      invokeDraw(&milk->code);
      haltOnError(console);
      break;
    default:
      break;
	}
	ticks++;
}
