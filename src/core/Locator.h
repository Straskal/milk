#ifndef _LOCATOR_H_
#define _LOCATOR_H_

#include "audio/SoundCache.h"
#include "audio/AudioPlayer.h"
#include "input/Keyboard.h"
#include "video/Window.h"
#include "video/Renderer.h"
#include "video/TextureCache.h"

/*
	The Locator's intended usage is for Lua modules.
*/
namespace milk {
	struct Locator {
		static Window* window;
		static Renderer* renderer;
		static Keyboard* keyboard;
		static TextureCache* textures;
		static SoundCache* audioCache;
		static AudioPlayer* audioPlayer;
	};
}

#endif