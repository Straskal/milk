#ifndef __CONSOLE__H__
#define __CONSOLE__H__

#include "milk.h"

#include <stdbool.h>

#define COMMAND_MAX_LENGTH 25

typedef enum {
  CONSOLE_INPUT_NONE    = 0 << 0,
  CONSOLE_INPUT_CHAR    = 1 << 0,
  CONSOLE_INPUT_BACK    = 1 << 1,
  CONSOLE_INPUT_ENTER   = 1 << 2,
  CONSOLE_INPUT_ESCAPE  = 1 << 3
} ConsoleInputState;

typedef struct {
  void (*startTextInput)();
  void (*stopTextInput)();

  ConsoleInputState state;
  ConsoleInputState previousState;
  char currentChar;
} ConsoleInput;

typedef enum {
  GAME,
  COMMAND
} ConsoleState;

typedef struct {
  ConsoleInput input;
  ConsoleState state;
  int lastErrorCount;
  char candidate[COMMAND_MAX_LENGTH];
  char previousCommand[COMMAND_MAX_LENGTH];
  int candidateLength;
  int previousCommandLength;
  bool isGameInitialized;
} Console;

Console *createConsole();
void freeConsole(Console *console);
void updateConsole(Console *console, Milk *milk);
void drawConsole(Console *console, Milk *milk);

#endif
