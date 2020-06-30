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

/**
 * Create a new instance of milk and initialize all submodules.
 * @return
 */
Milk *createMilk();

/**
 * Free milk and all of it's submodules.
 * @param milk
 */
void freeMilk(Milk *milk);

/**
 * Load scripts into memory.
 * @param milk
 */
void loadCode(Milk *milk);

/**
 * Unload all scripts from memory.
 * @param milk
 */
void unloadCode(Milk *milk);

/**
 * Invoke the _init callback in scripts.
 * @param code
 */
void invokeInit(Code *code);

/**
 * Invoke the _update callback in scripts.
 * @param code
 */
void invokeUpdate(Code *code);

/**
 * Invoke the _draw callback in scripts.
 * @param code
 */
void invokeDraw(Code *code);

#endif
