#ifndef _LOCATOR_H_
#define _LOCATOR_H_

#include "audio/AudioPlayer.h"
#include "audio/SoundCache.h"
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
		static Keyboard* keyboard;
		static Renderer* renderer;
		static TextureCache* textures;
		static AudioPlayer* audioPlayer;
		static SoundCache* sounds;
	};
}

#endif