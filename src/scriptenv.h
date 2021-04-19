#ifndef __API_H__
#define __API_H__

#include <stdbool.h>

#include "audio.h"
#include "input.h"
#include "video.h"

typedef struct Modules
{
    Input input;
    Video video;
    Audio audio;
} Modules;

typedef struct
{
    void *state;
} ScriptEnv;

void openScriptEnv(ScriptEnv *scriptEnv, Modules *modules);
void closeScriptEnv(ScriptEnv *scriptEnv);
bool loadEntryPoint(ScriptEnv *scriptEnv);
bool invokeInit(ScriptEnv *scriptEnv);
void invokeUpdate(ScriptEnv *scriptEnv);
void invokeDraw(ScriptEnv *scriptEnv);

#endif
