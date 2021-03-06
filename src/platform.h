#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdbool.h>

void platform_close();
void platform_lockAudioDevice();
void platform_unlockAudioDevice();
void platform_startTextInput();
void platform_stopTextInput();
bool platform_getCharInput(char *c);
bool platform_backspace();
void platform_toggleFullscreen();

#endif
