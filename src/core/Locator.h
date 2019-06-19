#ifndef _LOCATOR_H_
#define _LOCATOR_H_

#include "audio/AudioPlayer.h"
#include "audio/SoundCache.h"
#include "graphics/Renderer.h"
#include "graphics/TextureCache.h"
#include "keyboard/Keyboard.h"
#include "window/Window.h"

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