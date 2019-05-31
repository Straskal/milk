#include <iostream>

#include "SDL_main.h"
#include "SDL.h"

#include "milk.h"
#include "node/RootNode.h"
#include "script/lua/LuaEnvironment.h"
#include "window/sdl/Renderer.h"
#include "window/sdl/Window.h"

#define MILK_SUCCESS 0;
#define MILK_FAIL 1;

int main(int argc, char* argv[]) {
	milk::MilkState milkState;
	milkState.window = new milk::sdl::Window();
	milkState.renderer = new milk::sdl::Renderer();
	milkState.scriptenv = new milk::lua::LuaEnvironment();
	milkState.rootnode = new milk::RootNode();

	if (!milk::state::init(milkState)) {
		goto exit_fail;
	}

	try {
		for (int i = 0; i < 500; i++) {
			milkState.scriptenv->addScript(i, "res/player.lua");
		}

		while (milkState.running) {
			int frameStartTime = SDL_GetTicks();

			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					milkState.running = false;
				}
			}

			milk::state::tick(milkState);
			milk::state::postTick(milkState);
			milk::state::render(milkState);
			milk::state::postRender(milkState);

			Uint32 frameTime = SDL_GetTicks() - frameStartTime;
			if (frameTime < milk::state::MILLISECONDS_PER_FRAME) {
				SDL_Delay((Uint32)(milk::state::MILLISECONDS_PER_FRAME - frameTime));
			}				
		}
	}
	catch (std::exception e) {
		std::cout << "Fatal error encountered: " << e.what() << std::endl;
		goto exit_fail;
	}

	goto exit_success;

exit_success:
	milk::state::quit(milkState);
	delete milkState.rootnode; milkState.rootnode = nullptr;
	delete milkState.scriptenv; milkState.scriptenv = nullptr;
	delete milkState.renderer; milkState.renderer = nullptr;
	delete milkState.window; milkState.window = nullptr;
	return MILK_SUCCESS;

exit_fail:
	milk::state::quit(milkState);
	delete milkState.rootnode; milkState.rootnode = nullptr;
	delete milkState.scriptenv; milkState.scriptenv = nullptr;
	delete milkState.renderer; milkState.renderer = nullptr;
	delete milkState.window; milkState.window = nullptr;
	return MILK_SUCCESS;
}