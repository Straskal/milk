#ifndef MILK_H
#define MILK_H

#include <string>

namespace milk {
	class SceneGraph;
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
		Window* window;
		Renderer* renderer;
		ScriptEnvironment* scriptenv;
		SceneGraph* scene;
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