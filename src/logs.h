#ifndef __LOGS_H__
#define __LOGS_H__

#define MAX_LOGS       16
#define MAX_LOG_LENGTH 512

#ifndef MILK_CMD
#define LOG_INIT()          ((void)0)
#define LOG_INFO(text)      ((void)0)
#define LOG_WARN(text)      ((void)0)
#define LOG_ERROR(text)     ((void)0)
#define LOG_GET()           ((void)0)
#define LOG_CLEAR()         ((void)0)
#else
#define LOG_INIT()          initLogs()
#define LOG_INFO(text)      logMessage(text, INFO)
#define LOG_WARN(text)      logMessage(text, WARN)
#define LOG_ERROR(text)     logMessage(text, ERROR)
#define LOG_GET()           getLogs()
#define LOG_CLEAR()         clearLogs()
#endif

typedef enum logType
{
    INFO, WARN, ERROR
} LogType;

typedef struct logMessage
{
    LogType type;
    char    text[MAX_LOG_LENGTH];
} LogMessage;

typedef struct logs
{
    LogMessage  messages[MAX_LOGS];
    int         count;
    int         errorCount;
} Logs;

void initLogs();
void logMessage(const char *text, LogType type);
Logs *getLogs();
void clearLogs();

#endif
