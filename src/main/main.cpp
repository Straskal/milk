#include "SDL_main.h"
#include "SDL.h"

#include "milk.h"
#include "lua/LuaEnvironment.h"
#include "scene/Actors.h"
#include "window/Renderer.h"
#include "window/Window.h"
#include "window/adapter/RendererAdapter.h"
#include "window/adapter/WindowAdapter.h"

int main(int argc, char* argv[]) {
	milk::MilkState milkState;
	milkState.lua = new milk::LuaEnvironment();
	milkState.window = new milk::adapter::WindowAdapter();
	milkState.renderer = new milk::adapter::RendererAdapter();

	milk::state::init(milkState);

	while (true) {
		int frameStartTime = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				goto exit;
			}
		}

		milk::state::tick(milkState);
		milk::state::postTick(milkState);
		milk::state::render(milkState);

		Uint32 frameTime = SDL_GetTicks() - frameStartTime;
		if (frameTime < milk::state::MILLISECONDS_PER_FRAME)
			SDL_Delay((Uint32)(milk::state::MILLISECONDS_PER_FRAME - frameTime));
	}

exit:
	milk::state::quit(milkState);

	delete milkState.renderer;
	delete milkState.window;

	return 0;
}