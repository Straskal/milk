#ifndef __CONSOLE__H__
#define __CONSOLE__H__

#include "milk.h"

#include <stdbool.h>

#define COMMAND_MAX_LENGTH 25

typedef enum consoleInputState
{
  CONSOLE_INPUT_NONE    = 0 << 0,
  CONSOLE_INPUT_CHAR    = 1 << 0,
  CONSOLE_INPUT_BACK    = 1 << 1,
  CONSOLE_INPUT_ENTER   = 1 << 2,
  CONSOLE_INPUT_ESCAPE  = 1 << 3
} ConsoleInputState;

// The console handles input differently than the core engine.
// This creates some weird flow for the milk api but I can't think of a better way to hand this right now.
typedef struct consoleInput
{
  void (*startTextInput)();
  void (*stopTextInput)();

  ConsoleInputState state;
  ConsoleInputState previousState;
  char currentChar;
} ConsoleInput;

// When compiled with the console, the console pretty much takes over.
// The console determines what state the engine is in.
typedef enum consoleState
{
  GAME,
  COMMAND
} ConsoleState;

typedef struct console
{
  ConsoleInput input;
  ConsoleState state;
  int lastErrorCount;
  char commandCandidate[COMMAND_MAX_LENGTH];
  char previousCommand[COMMAND_MAX_LENGTH];
  int commandCandidateLength;
  int previousCommandLength;
  bool isGameInitialized;
} Console;

// Create new console and initialize commands.
//
Console *createConsole();

// Free the console
//
void freeConsole(Console *console);

// Update the console. Calling this could potentially update the game as well, depending on the consoles state.
//
void updateConsole(Console *console, Milk *milk);

// Draw the console.
//
void drawConsole(Console *console, Milk *milk);

#endif
