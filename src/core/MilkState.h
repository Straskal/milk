#ifndef _MILK_H_
#define _MILK_H_

struct lua_State;

namespace milk {
	class SDLWindow;
	class SDLMouse;
	class SDLKeyboard;
	class SDLRenderer;
	class SDLTextureCache;
	class SDLAudioPlayer;
	class SDLMusicCache;
	class SDLSoundCache;

	class MilkState {
	public:
		MilkState();

		int run();

	private:
		lua_State* m_lua;
		SDLWindow* m_window;
		SDLMouse* m_mouse;
		SDLKeyboard* m_keyboard;
		SDLRenderer* m_renderer;
		SDLTextureCache* m_textures;
		SDLAudioPlayer* m_audioPlayer;
		SDLSoundCache* m_sounds;
		SDLMusicCache* m_music;
	};
}

#endif