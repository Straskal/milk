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

// Create a new instance of milk and initialize all submodules.
//
Milk *createMilk();

// Free milk and all of it's submodules.
//
void freeMilk(Milk *milk);

// Load scripts into memory.
//
void loadCode(Milk *milk);

// Unload all scripts from memory.
//
void unloadCode(Milk *milk);

// Invoke the _init callback in scripts.
//
void invokeInit(Code *code);

// Invoke the _update callback in scripts.
//
void invokeUpdate(Code *code);

// Invoke the _draw callback in scripts.
//
void invokeDraw(Code *code);

#endif
