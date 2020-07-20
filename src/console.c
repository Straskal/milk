#include "common.h"
#include "console.h"
#include "logs.h"

#include <ctype.h>
#include <string.h>

#define VERSION_HEADER "MILK [1.0.0]"
#define LOG_START_HEIGHT 48
#define LOG_END_HEIGHT (224 - 32)
#define MAX_LINES ((LOG_END_HEIGHT - LOG_START_HEIGHT) / 8)
#define CHARS_PER_LINE 38

static unsigned int ticks = 0;

static const Color32 primary = 0xffffff;
static const Color32 dark = 0x5c5c5c;
static const Color32 alert = 0xbf4040;
static const Color32 warn = 0xffec27;

static void cmdUnload(Console *console, Milk *milk, char *argument) {
	UNUSED(console);
	UNUSED(argument);
	unloadCode(milk);
	unloadSound(&milk->audio, -1);
	closeStream(&milk->audio, -1);
	console->isGameInitialized = false;
	LOG_INFO("Game has been unloaded");
}

static void cmdClear(Console *console, Milk *milk, char *argument) {
	UNUSED(console);
	UNUSED(milk);
	UNUSED(argument);
	LOG_CLEAR();
	console->lastErrorCount = 0;
}

static void cmdQuit(Console *console, Milk *milk, char *argument) {
	UNUSED(console);
	UNUSED(argument);
	milk->shouldQuit = true;
}

typedef struct {
	char *cmd;
	void (*execute)(Console *, Milk *, char *);
} Command;

static Command commands[] = {
	{"unload", cmdUnload},
	{"clear", cmdClear},
	{"quit", cmdQuit},
};

static void resetCandidate(Console *console) {
	memset(console->candidate, 0, COMMAND_MAX_LENGTH);
	console->candidateLength = 0;
}

Console *createConsole() {
	Console *console = calloc(1, sizeof(Console));
	console->state = COMMAND;
	resetCandidate(console);
	return console;
}

void freeConsole(Console *console) {
	free(console);
}

static bool hasInputContinuous(ConsoleInput *input, ConsoleInputState inputState) {
	return IS_BIT_SET(input->state, inputState);
}

static bool hasInput(ConsoleInput *input, ConsoleInputState inputState) {
	ConsoleInputState currentState = input->state;
	ConsoleInputState previousState = input->previousState;
	return IS_BIT_SET(currentState, inputState) && !IS_BIT_SET(previousState, inputState);
}

static void handleBackspace(Console *console) {
	if (hasInputContinuous(&console->input, CONSOLE_INPUT_BACK) && console->candidateLength > 0)
		console->candidate[--console->candidateLength] = '\0';
}

static void handleCharacterInput(Console *console) {
	if (hasInputContinuous(&console->input, CONSOLE_INPUT_CHAR) && console->candidateLength < COMMAND_MAX_LENGTH - 1) 	{
		console->candidate[console->candidateLength] = console->input.currentChar;
		console->candidate[++console->candidateLength] = '\0';
	}
}

static void handleGoToPreviousCommand(Console *console, Milk *milk) {
	if (isButtonPressed(&milk->input, BTN_UP) && console->previousCommandLength > 0) {
		strcpy(console->candidate, console->previousCommand);
		console->candidateLength = console->previousCommandLength;
	}
}

static void handleClearCandidate(Console *console, Milk *milk) {
	if (isButtonPressed(&milk->input, BTN_DOWN))
		resetCandidate(console);
}

static void handleEnter(Console *console, Milk *milk) {
	if (hasInput(&console->input, CONSOLE_INPUT_ENTER) && console->candidateLength > 0) {
		int numCommands = sizeof(commands) / sizeof(Command);
		char tempCandidate[COMMAND_MAX_LENGTH];
		strcpy(tempCandidate, console->candidate);
		char *token = strtok(tempCandidate, " ");
		while (numCommands--) {
			if (strcmp(token, commands[numCommands].cmd) == 0) {
				token = strtok(NULL, " ");
				commands[numCommands].execute(console, milk, token);
				strcpy(console->previousCommand, console->candidate);
				console->previousCommandLength = console->candidateLength;
				resetCandidate(console);
				return;
			}
		}
		LOG_WARN("Unknown command");
		resetCandidate(console);
	}
}

static void handleEscape(Console *console, Milk *milk) {
	if (hasInput(&console->input, CONSOLE_INPUT_ESCAPE)) {
		if (console->state != COMMAND) {
			pauseSound(&milk->audio, -1);
			pauseStream(&milk->audio, -1);
			console->state = COMMAND;
			console->input.startTextInput();
		}	else {
			if (!console->isGameInitialized) {
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

static void handleInput(Console *console, Milk *milk) {
	handleBackspace(console);
	handleCharacterInput(console);
	handleGoToPreviousCommand(console, milk);
	handleClearCandidate(console, milk);
	handleEnter(console, milk);
}

static void haltOnError(Console *console) {
	if (console->lastErrorCount < LOG_GET()->errorCount) {
		console->lastErrorCount = LOG_GET()->errorCount;
		console->state = COMMAND;
		console->input.startTextInput();
	}
}

void updateConsole(Console *console, Milk *milk) {
	handleEscape(console, milk);
	switch (console->state) {
		case COMMAND:
			handleInput(console, milk);	break;
		case GAME:
			invokeUpdate(&milk->code);
			haltOnError(console);
			break;
		default: break;
	}
}

static void drawCommandLine(Console *console, Milk *milk) {
	Video *video = &milk->video;
	static const char *separator = "------------------------------";
	static const char *pointer = ">";
	int pointerWidth = fontWidth(pointer);
	int x = 8;
	int y = 10;
	blitFont(video, -1, x, y, VERSION_HEADER, 1, primary);
	blitFont(video, -1, x, (y += FONT_CHAR_HEIGHT), separator, 1, primary);
	blitFont(video, -1, x, (y += FONT_CHAR_HEIGHT * 2), pointer, 1, primary);
	blitFont(video, -1, (x += pointerWidth + 2), y, console->candidate, 1, primary);
	if (ticks % 64 < 48) blitFilledRectangle(video, x + fontWidth(console->candidate), y, 6, 8, alert);
}

static void drawPlayingIndicator(Console *console, Milk *milk) {
	const int size = 4;
	const int padding = 8;
	const int offset = size + padding;
	if (console->isGameInitialized) {
		blitFilledRectangle(&milk->video, FRAMEBUFFER_WIDTH - offset, offset, size, size, 0x00ff00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - offset, offset, 0x00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - padding - 1, offset, 0x00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - offset, offset + 3, 0x00);
		blitPixel(&milk->video, FRAMEBUFFER_WIDTH - padding - 1, offset + 3, 0x00);
	}
}

typedef struct {
	char text[CHARS_PER_LINE];
	Color32 color;
} ConsoleLine;

static Color32 getLogColor(LogType type) {
	switch (type) {
		case INFO: return dark;
		case WARN: return warn;
		case ERROR:	return alert;
		default: return dark;
	}
}

static void getLogLines(Logs *logs, ConsoleLine *lines, int *numLines) {
	int currentLine = 0;
	for (int i = logs->count - 1; i >= 0; i--) {
		if (currentLine == MAX_LINES - 1)	break;
		char tempText[MAX_LOG_LENGTH + 3] = ">:";
		strcpy(&tempText[2], logs->messages[i].text);
		char *splitByNewline = strtok(tempText, "\n"); // Split message by newline.
		while (splitByNewline != NULL && currentLine < MAX_LINES - 1) {
			size_t messageLength = strlen(splitByNewline);
			char *messageText = splitByNewline;
			while (messageLength > 0 && currentLine < MAX_LINES - 1) { // Draw the message in separate line.
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

static void drawLogLines(Milk *milk) {
	ConsoleLine lines[MAX_LINES];
	int numLines;
	getLogLines(LOG_GET(), lines, &numLines);
	for (int i = 0; i < numLines; i++)
		blitFont(&milk->video, -1, 8, LOG_START_HEIGHT + ((8 + 2) * i), lines[i].text, 1, lines[i].color);
}

void drawConsole(Console *console, Milk *milk) {
	switch (console->state) {
		case COMMAND:
			clearFramebuffer(&milk->video, 0x000000);
			drawCommandLine(console, milk);
			drawPlayingIndicator(console, milk);
			drawLogLines(milk);
			break;
		case GAME:
			invokeDraw(&milk->code);
			haltOnError(console);
			break;
		default: break;
	}
	ticks++;
}
