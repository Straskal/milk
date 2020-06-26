#include "logs.h"

#include <string.h>

static Logs logs;

void initLogs()
{
    for (int i = 0; i < MAX_LOGS; i++)
    {
        logs.messages[i].type = INFO;
        memset(logs.messages[i].text, 0, MAX_LOG_LENGTH);
    }

    logs.count = 0;
    logs.errorCount = 0;
}

static LogMessage *getNextFreeLogMessage()
{
    if (logs.count == MAX_LOGS)
    {
        for (int i = 0; i < MAX_LOGS - 1; i++)
            logs.messages[i] = logs.messages[i + 1];

        return &logs.messages[MAX_LOGS - 1];
    }
    else
        return &logs.messages[logs.count++];
}

void logMessage(const char *text, LogType type)
{
    size_t len = strlen(text);

    if (len > MAX_LOG_LENGTH)
        len = MAX_LOG_LENGTH;

    if (type == ERROR)
        logs.errorCount++;

    LogMessage *newLogMessage = getNextFreeLogMessage();
    memset(newLogMessage->text, 0, MAX_LOG_LENGTH);
    strncpy(newLogMessage->text, text, len);
    newLogMessage->type = type;
}

Logs *getLogs()
{
    return &logs;
}

void clearLogs()
{
    logs.count = 0;
    logs.errorCount = 0;
}
