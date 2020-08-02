#ifndef __MILK_H__
#define __MILK_H__

#include "scriptenv.h"

typedef struct
{
	Modules modules;
	ScriptEnv scripts;

#ifdef BUILD_WITH_CONSOLE
#define COMMAND_MAX_LENGTH 36
	struct Console
	{
		char candidate[COMMAND_MAX_LENGTH];
		int candidateLength;
		int ticks;
		bool isEnabled;
	} console;
#endif
} Milk;

Milk *createMilk();
void freeMilk(Milk *milk);
void initializeMilk(Milk *milk);
void updateMilk(Milk *milk);
void drawMilk(Milk *milk);

#endif
