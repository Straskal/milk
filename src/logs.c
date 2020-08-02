#include <string.h>

#include "logs.h"

static struct {
  char error[1024];
} LogContext;

void logError(const char *message) {
  strcpy(LogContext.error, message);
}

bool hasError() {
  return strlen(LogContext.error) > 0;
}

const char *getError() {
  return LogContext.error;
}

void clearError() {
  memset(LogContext.error, 0, sizeof(LogContext.error));
}
