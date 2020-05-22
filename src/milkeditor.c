#include "milkeditor.h"
#include "milkapi.h"

#include <string.h>

#define COMMAND_QUEUE_MAX 25

static char *commands[COMMAND_QUEUE_MAX];
static char command[32];
static int len = 2;
static int ticks = 0;
static char *result;

typedef enum
{
	ENTRY,
	COMMAND_LINE
} State;

static State _state = COMMAND_LINE;
static State _next;

void milkEditorOpen(Milk *milk)
{
	command[0] = '>';
	command[1] = ':';
	len = 2;
	for (int i = len; i < 32; i++)
		command[i] = ' ';
	milk->system.startTextInput();
}

void milkEditorLoop(Milk *milk)
{
	milkResetDrawState(&milk->video);

	switch (_state)
	{
	case COMMAND_LINE:
	{
		char ch;
		if (milk->system.backspace() && len > 2)
			command[len--] = ' ';

		if (milk->system.readTextInput(&ch))
			command[len++] = ch;

		command[len] = ticks % 32 > 16 == 0 ? '_' : ' ';
		milkClear(&milk->video, 0x1a1a1a);
		milkSpriteFont(&milk->video, 10, 10, "MILK v1.0", 1);
		milkSpriteFont(&milk->video, 10, 26, "---------------------------", 1);
		milkSpriteFont(&milk->video, 10, 40, command, 1);
		milkSpriteFont(&milk->video, 10, 62, result, 1);

		if (milk->system.enter())
		{
			command[len] = '\0';
			if (strcmp("reload", &command[2]) == 0)
			{
				milkUnloadScripts(milk);
				milkLoadScripts(milk);
				result = "Scripts have been reloaded.";
			}
			else
			{
				result = "Unknown command.";
			}

			command[0] = '>';
			command[1] = ':';
			len = 2;
			for (int i = len; i < 32; i++)
				command[i] = ' ';
		}
		break;
	}
	default:
		break;
	}

	ticks++;
}

void milkEditorClose(Milk *milk)
{
	result = NULL;
	milk->system.stopTextInput();
}
