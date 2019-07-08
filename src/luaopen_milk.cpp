#include "luaopen_milk.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "SDL.h"

#include "luamlib.h"
#include "State.h"

#include "audio/audio.h"
#include "graphics/graphics.h"
#include "window/window.h"

#include "keyboard/sdl/SDLKeyboard.h"
#include "mouse/sdl/SDLMouse.h"

#define free_ptr(x) delete x; x = nullptr
#define deinit_and_free_ptr(x) x->free(); free_ptr(x)

static milk::SDLMouse* mouse = nullptr;
static milk::SDLKeyboard* keyboard = nullptr;

static int milk_init(lua_State* L)
{
	mouse = new milk::SDLMouse();
	keyboard = new milk::SDLKeyboard();

	milk::State::mouse = mouse;
	milk::State::keyboard = keyboard;

	if (milk::window_init()
		&& milk::graphics_init(milk::window_get_handle())
		&& milk::audio_init()) {

		return 0;
	}
	return luaL_error(L, "could not start milk!");
}

// TODO: MOVE TO WINDOW
static int milk_poll(lua_State* L)
{
	(void)L;

	mouse->reset();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			milk::window_close();
		}
		mouse->handleEvent(&event);
	}

	mouse->tick();
	keyboard->tick();
	return 0;

}

static int milk_quit(lua_State* L)
{
	milk::State::mouse = nullptr;
	milk::State::keyboard = nullptr;

	free_ptr(keyboard);
	free_ptr(mouse);
	milk::audio_quit();
	milk::graphics_quit();
	milk::window_quit();
	return 0;
}

static const luaL_Reg milk_funcs[] = {
	{ "init", milk_init },
	{ "poll", milk_poll },
	{ "quit", milk_quit },
	{ nullptr, nullptr }
};

int luaopen_milk(lua_State* L)
{
	milk::luaM_openlibs(L);
	luaL_newlib(L, milk_funcs);
	return 1;
}