#ifndef __MILK_H__
#define __MILK_H__

#include "api.h"

typedef struct {
	Modules modules;
	Scripts scripts;
} Milk;

Milk *createMilk();
void freeMilk(Milk *milk);
void initializeMilk(Milk *milk);
void updateMilk(Milk *milk);
void drawMilk(Milk *milk);

#endif
