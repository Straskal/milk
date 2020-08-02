#ifndef __MILK_H__
#define __MILK_H__

#include "api.h"

typedef struct {
	Modules modules;
	Scripts scripts;

#ifdef BUILD_WITH_CONSOLE
#define COMMAND_MAX_LENGTH 36
	struct Console {
		int ticks;
		bool isEnabled;
		char candidate[COMMAND_MAX_LENGTH];
		int candidateLength;
		int lastErrorCount;
	} console;
#endif
} Milk;

Milk *createMilk();
void freeMilk(Milk *milk);
void initializeMilk(Milk *milk);
void updateMilk(Milk *milk);
void drawMilk(Milk *milk);

#endif
