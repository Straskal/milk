#include <iostream>

#include "SDL_main.h"
#include "SDL.h"

#include "milk.h"
#include "lua/adapter/LuaEnvironmentAdapter.h"
#include "scene/Actors.h"
#include "window/adapter/RendererAdapter.h"
#include "window/adapter/WindowAdapter.h"

#define MILK_SUCCESS 0;
#define MILK_FAIL 1;

int main(int argc, char* argv[]) {
	milk::MilkState milkState;
	milkState.luaenv = new milk::adapter::LuaEnvironmentAdapter();
	milkState.window = new milk::adapter::WindowAdapter();
	milkState.renderer = new milk::adapter::RendererAdapter();

	if (!milk::state::init(milkState)) {
		goto exit_fail;
	}

	try {
		for (int i = 0; i < 500; i++) {
			milkState.luaenv->addScript(i, "res/player.lua");
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
	delete milkState.luaenv; milkState.luaenv = nullptr;
	delete milkState.renderer; milkState.renderer = nullptr;
	delete milkState.window; milkState.window = nullptr;
	return MILK_SUCCESS;

exit_fail:
	milk::state::quit(milkState);
	delete milkState.luaenv; milkState.luaenv = nullptr;
	delete milkState.renderer; milkState.renderer = nullptr;
	delete milkState.window; milkState.window = nullptr;
	return MILK_SUCCESS;
}