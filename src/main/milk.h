#ifndef MILK_H
#define MILK_H

#include <string>

namespace milk {
	class Actors;
	class ScriptEnvironment;
	class Renderer;
	class Window;	   	 

	struct MilkStartupConfig {
		std::string winTitle;
		int winWidth;
		int winHeight;
		bool winFullscreen;
		int resWidth;
		int resHeight;
	};

	struct MilkState {
		ScriptEnvironment* scriptenv;
		Window* window;
		Renderer* renderer;
		Actors* actors;
		bool running;
	};

	namespace state {
		const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16

		bool init(MilkState& milkState);
		void tick(MilkState& milkState);
		void postTick(MilkState& milkState);
		void postRender(MilkState& milkState);
		void render(MilkState& milkState);
		void quit(MilkState& milkState);
	}
}

#endif