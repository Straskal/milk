#include <string.h>

#include "common.h"
#include "logs.h"
#include "milk.h"
#include "platform.h"

#ifdef BUILD_WITH_CONSOLE
#define CONSOLE_Y 180

typedef struct Console Console;
typedef enum State State;

static void __resetCandidate(Milk *milk) {
	memset(milk->console.candidate, 0, COMMAND_MAX_LENGTH);
	milk->console.candidateLength = 0;
}

static void __toggleConsole(Milk *milk) {
	milk->console.isEnabled = !milk->console.isEnabled;
	if (milk->console.isEnabled) {
		__resetCandidate(milk);
		pauseSound(&milk->modules.audio, -1);
		pauseStream(&milk->modules.audio);
		platform_startTextInput();
	} else {
		resumeSound(&milk->modules.audio, -1);
		resumeStream(&milk->modules.audio);
		platform_stopTextInput();
	}
}

static void __cmdReload(Milk *milk, char *argument) {
	UNUSED(argument);
	closeScriptEnv(&milk->scripts);
	openScriptEnv(&milk->scripts, &milk->modules);
	loadEntryPoint(&milk->scripts);
	invokeInit(&milk->scripts);
	__toggleConsole(milk);
}

static void __cmdQuit(Milk *milk, char *argument) {
	UNUSED(milk);
	UNUSED(argument);
	platform_close();
}

typedef struct {
	char *cmd;
	void (*execute)(Milk *, char *);
} Command;

static Command commands[] = {
	{"reload", __cmdReload},
	{"quit", __cmdQuit},
};

static void __initializeConsole(Milk *milk) {
	memset(&milk->console, 0, sizeof(milk->console));
}

static void __disableConsole(Milk *milk) {
	memset(&milk->console, 0, sizeof(milk->console));
}

static void __updateConsole(Milk *milk) {
	if (isExtDown(&milk->modules.input, INPUT_BACK) && milk->console.candidateLength > 0)
		milk->console.candidate[--milk->console.candidateLength] = '\0';
	if (isExtDown(&milk->modules.input, INPUT_CHAR) && milk->console.candidateLength < COMMAND_MAX_LENGTH - 1) {
		milk->console.candidate[milk->console.candidateLength] = milk->modules.input.extended.inChar;
		milk->console.candidate[++milk->console.candidateLength] = '\0';
	}
	if (isButtonPressed(&milk->modules.input, BTN_DOWN))
		__resetCandidate(milk);
	if (isExtPressed(&milk->modules.input, INPUT_ENTER)) {
		int numCommands = sizeof(commands) / sizeof(Command);
		char tempCandidate[COMMAND_MAX_LENGTH];
		strcpy(tempCandidate, milk->console.candidate);
		char *token = strtok(tempCandidate, " ");
		while (numCommands--) {
			if (strcmp(token, commands[numCommands].cmd) == 0)
			{
				token = strtok(NULL, " ");
				commands[numCommands].execute(milk, token);
				__resetCandidate(milk);
				return;
			}
		}
		__resetCandidate(milk);
	}
}

static void __drawConsole(Milk *milk) {
	setClip(&milk->modules.video, 0, CONSOLE_Y, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT - CONSOLE_Y);
	clearFramebuffer(&milk->modules.video, 0x1d2b53);
	drawRect(&milk->modules.video, 0, CONSOLE_Y, FRAMEBUFFER_WIDTH - 1, FRAMEBUFFER_HEIGHT - CONSOLE_Y - 1, 0xffffff);
	drawFont(&milk->modules.video, NULL, 5, CONSOLE_Y + 5, milk->console.candidate, 1, 0xffffff);
	if (milk->console.ticks % 64 < 48)
		drawFilledRect(&milk->modules.video, 5 + getFontWidth(milk->console.candidate), CONSOLE_Y + 5, 6, 8, 0xbf4040);
}

#endif

Milk *createMilk() {
	LOG_INIT();
	Milk *milk = malloc(sizeof(Milk));
	initializeInput(&milk->modules.input);
	initializeVideo(&milk->modules.video);
	initializeAudio(&milk->modules.audio);

#ifdef BUILD_WITH_CONSOLE
	__initializeConsole(milk);
#endif

	openScriptEnv(&milk->scripts, &milk->modules);
	return milk;
}

void freeMilk(Milk *milk) {
	closeScriptEnv(&milk->scripts);

#ifdef BUILD_WITH_CONSOLE
	__disableConsole(milk);
#endif

	disableAudio(&milk->modules.audio);
	disableVideo(&milk->modules.video);
	free(milk);
}

void initializeMilk(Milk *milk) {
	loadEntryPoint(&milk->scripts);
	invokeInit(&milk->scripts);
}

void updateMilk(Milk *milk) {
#ifdef BUILD_WITH_CONSOLE
	if (isExtPressed(&milk->modules.input, INPUT_ESCAPE))
		__toggleConsole(milk);
	if (!milk->console.isEnabled)
		invokeUpdate(&milk->scripts);
	else
		__updateConsole(milk);
#else
	invokeUpdate(&milk->scripts);
#endif
}

void drawMilk(Milk *milk) {
	resetDrawState(&milk->modules.video);
	#ifdef BUILD_WITH_CONSOLE
	if (!milk->console.isEnabled)
		invokeDraw(&milk->scripts);
	else
		__drawConsole(milk);

	milk->console.ticks++;
#else
	resetDrawState(&milk->modules.video);
	invokeDraw(&milk->scripts);
#endif
}
