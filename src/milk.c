#include "logs.h"
#include "milk.h"

Milk *createMilk() {
	LOG_INIT();
	Milk *milk = malloc(sizeof(Milk));
	initializeInput(&milk->modules.input);
	initializeVideo(&milk->modules.video);
	initializeAudio(&milk->modules.audio);
	openScriptEnv(&milk->scripts, &milk->modules);
	return milk;
}

void freeMilk(Milk *milk) {
	closeScriptEnv(&milk->scripts);
	disableAudio(&milk->modules.audio);
	disableVideo(&milk->modules.video);
	free(milk);
}

void initializeMilk(Milk *milk) {
	loadEntryPoint(&milk->scripts);
	invokeInit(&milk->scripts);
}

void updateMilk(Milk *milk) {
	invokeUpdate(&milk->scripts);
}

void drawMilk(Milk *milk) {
	resetDrawState(&milk->modules.video);
	invokeDraw(&milk->scripts);
}
