#include "milkmain.h"
#include "milkapi.h"
#include "milkeditor.h"

typedef enum
{
	ENTRY,
	GAME,
	EDITOR
} State;

static State state = ENTRY;
static State next = EDITOR;

static char **commands[10];
static int len;
static int started = 0;

void milkRunLoop(Milk *milk)
{
	if (!started)
	{
		milkLoadScripts(milk);
		started = 1;
	}

	if (milk->system.escape())
	{
		if (state == EDITOR)
			next = GAME;
		else
			next = EDITOR;
	}

	if (state != next)
	{
		switch (state)
		{
			case GAME:
				break;
			case EDITOR:
				milkEditorClose(milk);
				break;
			default:
				break;
		}

		state = next;

		switch (state)
		{
			case GAME:
				break;
			case EDITOR:
				milkEditorOpen(milk);
				break;
			default:
				break;
		}
	}

	switch (state)
	{
		case GAME:
			milkResetDrawState(&milk->video);
			milkInvokeUpdate(&milk->code);
			milkInvokeDraw(&milk->code);
			break;
		case EDITOR:
			milkResetDrawState(&milk->video);
			milkEditorLoop(milk);
			break;
	}
}
