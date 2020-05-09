#ifndef __MILK_API__
#define __MILK_API__

#include "milk.h"

void milk_load_code(MilkMachine* code);
void milk_invoke_code_upd(Code* code);
void milk_invoke_code_draw(Code* code);

#endif