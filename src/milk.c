#include <string.h>

#include "common.h"
#include "logs.h"
#include "milk.h"
#include "platform.h"

#ifdef BUILD_WITH_CONSOLE
#define CONSOLE_Y (FRAMEBUFFER_HEIGHT - 36)

typedef struct Console Console;
typedef enum State State;

static void __resetCandidate(Console *console)
{
	memset(console->candidate, 0, COMMAND_MAX_LENGTH);
	console->candidateLength = 0;
}

static void __toggleConsole(Milk *milk)
{
	Console *console = &milk->console;
	Modules *modules = &milk->modules;

	if (!(console->isEnabled && hasError()))
	{
		console->isEnabled = !console->isEnabled;
		if (console->isEnabled)
		{
			__resetCandidate(console);
			pauseSound(&modules->audio, -1);
			pauseStream(&modules->audio);
			platform_startTextInput();
		}
		else
		{
			resumeSound(&modules->audio, -1);
			resumeStream(&modules->audio);
			platform_stopTextInput();
		}
	}
}

static void __cmdReload(Milk *milk)
{
	ScriptEnv *env = &milk->scripts;
	Modules *modules = &milk->modules;

	clearError();
	closeScriptEnv(env);
	openScriptEnv(env, modules);
	loadEntryPoint(env);
	invokeInit(env);
	__toggleConsole(milk);
}

static void __cmdFullscreen(Milk *milk)
{
	UNUSED(milk);
	platform_toggleFullscreen();
}

static void __cmdQuit(Milk *milk)
{
	UNUSED(milk);
	platform_close();
}

typedef struct
{
	char *cmd;
	void (*execute)(Milk *);
} Command;

static Command commands[] =
{
	{"reload", __cmdReload},
	{"fullscreen", __cmdFullscreen},
	{"quit", __cmdQuit},
};

static void __initializeConsole(Milk *milk)
{
	memset(&milk->console, 0, sizeof(milk->console));
}

static void __disableConsole(Milk *milk)
{
	memset(&milk->console, 0, sizeof(milk->console));
}

static void __executeCommand(Milk *milk)
{
	int numCommands = sizeof(commands) / sizeof(Command);

	while (numCommands--)
	{
		if (strcmp(milk->console.candidate, commands[numCommands].cmd) == 0)
		{
			commands[numCommands].execute(milk);
			break;
		}
	}
	__resetCandidate(&milk->console);
}

static void __updateConsole(Milk *milk)
{
	Console *console = &milk->console;
	Modules *modules = &milk->modules;

	if (isExtDown(&modules->input, INPUT_BACK) && console->candidateLength > 0)
	{
		console->candidate[--console->candidateLength] = '\0';
	}
	if (isExtDown(&modules->input, INPUT_CHAR) && console->candidateLength < COMMAND_MAX_LENGTH - 1)
	{
		console->candidate[console->candidateLength] = modules->input.extended.inChar;
		console->candidate[++console->candidateLength] = '\0';
	}
	if (isButtonPressed(&modules->input, BTN_DOWN))
	{
		__resetCandidate(console);
	}
	if (isExtPressed(&modules->input, INPUT_ENTER))
	{
		__executeCommand(milk);
	}
}

static void __drawPanel(Video *video, const char *title, int x, int y, int w, int h)
{
	setClip(video, x, y, w, h);
	clearFramebuffer(video, 0x40318d);
	drawRect(video, x, y, w - 1, h - 1, 0xffffff);
	drawFont(video, NULL, x + 5, y + 5, title, 1, 0x7869c4);
}

static void __drawConsole(Milk *milk)
{
	Console *console = &milk->console;
	Video *video = &milk->modules.video;

	// Error panel
	if (hasError())
	{
		__drawPanel(video, "ERROR", 0, CONSOLE_Y - 79, FRAMEBUFFER_WIDTH, 80);
		drawWrappedFont(video, NULL, 5, CONSOLE_Y - 79 + 20, FRAMEBUFFER_WIDTH - 10, getError(), 1, 0xffbf4040);
	}

	// Console panel
	__drawPanel(video, "TERMINAL", 0, CONSOLE_Y, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT - CONSOLE_Y);
	drawFont(video, NULL, 5, CONSOLE_Y + 20, "~", 1, 0xffffff);
	drawFont(video, NULL, 18, CONSOLE_Y + 20, console->candidate, 1, 0xffffff);
	if (console->ticks % 64 < 48)
	{
		int candidateWidth = getFontWidth(console->candidate);
		drawFilledRect(video, 18 + candidateWidth, CONSOLE_Y + 20, 6, 8, 0xbf4040);
	}
}

#endif // BUILD_WITH_CONSOLE

Milk *createMilk()
{
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

void freeMilk(Milk *milk)
{
	closeScriptEnv(&milk->scripts);

#ifdef BUILD_WITH_CONSOLE
	__disableConsole(milk);
#endif

	disableAudio(&milk->modules.audio);
	free(milk);
}

void initializeMilk(Milk *milk)
{
	loadEntryPoint(&milk->scripts);
	invokeInit(&milk->scripts);
}

void updateMilk(Milk *milk)
{
#ifdef BUILD_WITH_CONSOLE
	if (hasError() && !milk->console.isEnabled)
		__toggleConsole(milk);
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

void drawMilk(Milk *milk)
{
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
