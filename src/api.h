#ifndef __API_H__
#define __API_H__

#include "audio.h"
#include "gamepad.h"
#include "video.h"

typedef struct Modules {
  Input input;
  Video video;
  Audio audio;
} Modules;

typedef struct {
  void *state;
} Scripts;

void openScriptEnv(Scripts *scripts, Modules *modules);
void closeScriptEnv(Scripts *scripts);
void loadEntryPoint(Scripts *scripts);
void invokeInit(Scripts *scripts);
void invokeUpdate(Scripts *scripts);
void invokeDraw(Scripts *scripts);

#endif
