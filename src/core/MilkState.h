#ifndef MILK_H
#define MILK_H

#include <string>

struct lua_State;

namespace milk {
	class SDLRenderer;
	class SDLWindow;
	class SDLKeyboard;

	class MilkState {
	public:
		MilkState();

		int run(const std::string& configPath);

	private:
		lua_State* m_lua;
		SDLWindow* m_window;
		SDLRenderer* m_renderer;
		SDLKeyboard* m_keyboard;
		bool m_running;
	};
}

#endif