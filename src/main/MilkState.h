#ifndef MILK_H
#define MILK_H

#include <string>

struct lua_State;

namespace milk {
	class Renderer;
	class Window;

	class MilkState {
	public:
		MilkState();

		int run(const std::string& configPath);

	private:
		lua_State* m_lua;
		Window* m_window;
		Renderer* m_renderer;
		bool m_running;
	};
}

#endif