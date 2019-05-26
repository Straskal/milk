#include <iostream>

#include "SDL_main.h"
#include "SDL.h"

#include "milk.h"
#include "lua/LuaEnvironment.h"
#include "scene/Actors.h"
#include "window/adapter/RendererAdapter.h"
#include "window/adapter/WindowAdapter.h"

#define MILK_SUCCESS 0;

int main(int argc, char* argv[]) {
	milk::MilkState milkState;
	milkState.lua = new milk::LuaEnvironment();
	milkState.window = new milk::adapter::WindowAdapter();
	milkState.renderer = new milk::adapter::RendererAdapter();

	if (!milk::state::init(milkState))
		goto quit;

	try {
		while (true) {
			int frameStartTime = SDL_GetTicks();

			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					goto quit;
				}
			}

			milk::state::tick(milkState);
			milk::state::postTick(milkState);
			milk::state::render(milkState);

			Uint32 frameTime = SDL_GetTicks() - frameStartTime;
			if (frameTime < milk::state::MILLISECONDS_PER_FRAME)
				SDL_Delay((Uint32)(milk::state::MILLISECONDS_PER_FRAME - frameTime));
		}
	}
	catch (std::exception e) {
		std::cout << "Fatal error encountered: " << e.what() << std::endl;
		goto quit;
	}

quit:
	milk::state::quit(milkState);
	delete milkState.lua; milkState.lua = nullptr;
	delete milkState.renderer; milkState.renderer = nullptr;
	delete milkState.window; milkState.window = nullptr;
	return MILK_SUCCESS;
}