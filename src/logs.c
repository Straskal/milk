#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "logs.h"

static char error[1024];

void logErrorf(const char *format, ...)
{
  memset(error, 0, sizeof(error));
  va_list va;
  va_start(va, format);
  vsprintf(error, format, va);
  va_end(va);
}

void logError(const char *message)
{
  memset(error, 0, sizeof(error));
  strcpy(error, message);
}

bool hasError()
{
  return strlen(error) > 0;
}

const char *getError()
{
  return error;
}

void clearError()
{
  memset(error, 0, sizeof(error));
}
