#ifndef _MILK_H_
#define _MILK_H_

#include <string>

struct lua_State;

namespace milk {
	class SDLRenderer;
	class SDLWindow;
	class SDLKeyboard;
	class SDLTextureCache;

	class MilkState {
	public:
		MilkState();

		int run(const std::string& configPath);

	private:
		lua_State* m_lua;
		SDLWindow* m_window;
		SDLRenderer* m_renderer;
		SDLKeyboard* m_keyboard;
		SDLTextureCache* m_textures;
	};
}

#endif