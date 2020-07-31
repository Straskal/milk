#ifndef __PLATFORM_H__
#define __PLATFORM_H__

typedef enum {
  PLATFORM_QUIT = 1 << 0,
} PlatformFlags;

typedef struct {
  PlatformFlags flags;
  void (*startTextInput)();
	void (*stopTextInput)();
  void (*lockAudioDevice)();
	void (*unlockAudioDevice)();
} Platform;

Platform *getPlatform();

#endif
