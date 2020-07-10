#ifndef __MILK_H__
#define __MILK_H__

#include <stdbool.h>

#include "audio.h"
#include "common.h"
#include "gamepad.h"
#include "video.h"

typedef struct code
{
	void *state;
} Code;

typedef struct milk
{
	Input input;
	Video video;
	Audio audio;
	Code  code;
	bool  shouldQuit;
} Milk;

Milk *createMilk();
void freeMilk(Milk *milk);
void loadCode(Milk *milk);
void unloadCode(Milk *milk);
void invokeInit(Code *code);
void invokeUpdate(Code *code);
void invokeDraw(Code *code);

#endif
