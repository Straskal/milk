#ifndef _LOCATOR_H_
#define _LOCATOR_H_

/*
	The Locator's intended usage is for Lua modules.
*/
namespace milk {
	class Window;
	class Mouse;
	class Keyboard;
	class Renderer;
	class TextureCache;
	class AudioPlayer;
	class MusicCache;
	class SoundCache;

	struct Locator {
		static Window* window;
		static Mouse* mouse;
		static Keyboard* keyboard;
		static Renderer* renderer;
		static TextureCache* textures;
		static AudioPlayer* audioPlayer;
		static MusicCache* music;
		static SoundCache* sounds;
	};
}

#endif