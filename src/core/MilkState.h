#ifndef _MILK_H_
#define _MILK_H_

#include <string>

struct lua_State;

namespace milk {
	class SDLAudioPlayer;
	class SDLMusicCache;
	class SDLSoundCache;
	class SDLRenderer;
	class SDLWindow;
	class SDLKeyboard;
	class SDLTextureCache;

	class MilkState {
	public:
		MilkState();

		int run();

	private:
		lua_State* m_lua;
		SDLWindow* m_window;
		SDLRenderer* m_renderer;
		SDLKeyboard* m_keyboard;
		SDLTextureCache* m_textures;
		SDLAudioPlayer* m_audioPlayer;
		SDLSoundCache* m_sounds;
		SDLMusicCache* m_music;
	};
}

#endif