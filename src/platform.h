#ifndef __PLATFORM_H__
#define __PLATFORM_H__

void platform_close();
void platform_lockAudioDevice();
void platform_unlockAudioDevice();
void platform_startTextInput();
void platform_stopTextInput();

#endif
