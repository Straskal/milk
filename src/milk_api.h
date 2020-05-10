#ifndef __MILK_API__
#define __MILK_API__

#include "milk.h"

void milkLoadScripts(Milk* code);
void milkInvokeUpdate(Code* code);
void milkInvokeDraw(Code* code);

#endif