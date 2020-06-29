#include "logs.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

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

void logMessage(LogType type, const char *text, ...)
{
	size_t len = strlen(text);
	if (len > MAX_LOG_LENGTH) len = MAX_LOG_LENGTH;
	if (type == ERROR) logs.errorCount++;
	LogMessage *newLogMessage = getNextFreeLogMessage();
	newLogMessage->type = type;
	memset(newLogMessage->text, 0, MAX_LOG_LENGTH);

	va_list args;
	va_start(args, text);
	snprintf(newLogMessage->text, len, text, args);
	va_end(args);
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
