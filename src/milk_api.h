#ifndef __MILK_API__
#define __MILK_API__

#include "milk.h"

void MilkLoadCode(MilkMachine* code);
void MilkInvokeCodeUpdate(Code* code);
void MilkInvokeCodeDraw(Code* code);

#endif