#ifndef MILK_H
#define MILK_H

#include <string>

struct lua_State;

namespace milk {
	namespace sdl {
		class Renderer;
		class Window;
		class Keyboard;
	}

	class MilkState {
	public:
		MilkState();

		int run(const std::string& configPath);

	private:
		lua_State* m_lua;
		sdl::Window* m_window;
		sdl::Renderer* m_renderer;
		sdl::Keyboard* m_keyboard;
		bool m_running;
	};
}

#endif