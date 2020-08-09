#ifndef __LOGS_H__
#define __LOGS_H__

#include <stdbool.h>

void logErrorf(const char *format, ...);
void logError(const char *message);
bool hasError();
const char *getError();
void clearError();

#endif
