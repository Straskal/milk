#ifndef __LOGS_H__
#define __LOGS_H__

#define MAX_LOGS       16
#define MAX_LOG_LENGTH 512

#ifdef BUILD_WITH_CONSOLE
#define LOG_INIT()              initializeLogs()
#define LOG_INFO(text)          logMessage(INFO,    text)
#define LOG_WARN(text)          logMessage(WARN,    text)
#define LOG_ERROR(text)         logMessage(ERROR,   text)
#define LOG_INFOF(text, ...)    logMessage(INFO,    text, __VA_ARGS__)
#define LOG_WARNF(text, ...)    logMessage(WARN,    text, __VA_ARGS__)
#define LOG_ERRORF(text, ...)   logMessage(ERROR,   text, __VA_ARGS__)
#define LOG_GET()               getLogs()
#define LOG_CLEAR()             clearLogs()
#else
#define LOG_INIT()              ((void)0)
#define LOG_INFO(text)          ((void)0)
#define LOG_WARN(text)          ((void)0)
#define LOG_ERROR(text)         ((void)0)
#define LOG_INFOF(text, ...)    ((void)0)
#define LOG_WARNF(text, ...)    ((void)0)
#define LOG_ERRORF(text, ...)   ((void)0)
#define LOG_GET()               ((void)0)
#define LOG_CLEAR()             ((void)0)
#endif

typedef enum logType
{
  INFO, WARN, ERROR
} LogType;

typedef struct logMessage
{
  char text[MAX_LOG_LENGTH];
  LogType type;
} LogMessage;

// This is really only used for the console at the moment.
// It would be nice to be able to dump the logs to a text file.
//
typedef struct logs
{
  LogMessage messages[MAX_LOGS];
  int count;
  int errorCount;
} Logs;

// Initialize the logger.
//
void initializeLogs();

// Log a formatted message of the given type.
//
void logMessage(LogType type, const char *text, ...);

// Returns all current logs.
//
Logs *getLogs();

// Clears all logs.
//
void clearLogs();

#endif
