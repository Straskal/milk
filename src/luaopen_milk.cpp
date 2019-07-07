#include "luaopen_milk.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "SDL.h"

#include "luamlib.h"
#include "State.h"

#include "graphics/graphics.h"
#include "window/window.h"

#include "audio/sdl/SDLAudioPlayer.h"
#include "audio/sdl/SDLMusicCache.h"
#include "audio/sdl/SDLSoundCache.h"
#include "keyboard/sdl/SDLKeyboard.h"
#include "mouse/sdl/SDLMouse.h"
#include "time/sdl/SDLTime.h"

#define free_ptr(x) delete x; x = nullptr
#define deinit_and_free_ptr(x) x->free(); free_ptr(x)

static milk::SDLTime* time = nullptr;
static milk::SDLMouse* mouse = nullptr;
static milk::SDLKeyboard* keyboard = nullptr;
static milk::SDLAudioPlayer* audio_player = nullptr;
static milk::SDLSoundCache* sound_cache = nullptr;
static milk::SDLMusicCache* music_cache = nullptr;

static int milk_init(lua_State* L)
{
	if (milk::State::initialized) {
		return luaL_error(L, "milk has already been initialized!");
	}

	time = new milk::SDLTime();
	audio_player = new milk::SDLAudioPlayer();
	music_cache = new milk::SDLMusicCache();
	sound_cache = new milk::SDLSoundCache();
	mouse = new milk::SDLMouse();
	keyboard = new milk::SDLKeyboard();

	milk::State::time = time;
	milk::State::audioPlayer = audio_player;
	milk::State::sounds = sound_cache;
	milk::State::music = music_cache;
	milk::State::mouse = mouse;
	milk::State::keyboard = keyboard;

	if (milk::window_init()
		&& milk::graphics_init(milk::window_get_handle())
		&& audio_player->init()) {

		time->start();
		milk::State::initialized = true;
		return 0;
	}
	return luaL_error(L, "could not start milk!");
}

// TODO MOVE TO WINDOW
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
	if (!milk::State::initialized) {
		return luaL_error(L, "you cannot quit milk when it has not been initialized!");
	}

	milk::State::time = nullptr;
	milk::State::window = nullptr;
	milk::State::renderer = nullptr;
	milk::State::images = nullptr;
	milk::State::audioPlayer = nullptr;
	milk::State::sounds = nullptr;
	milk::State::music = nullptr;
	milk::State::mouse = nullptr;
	milk::State::keyboard = nullptr;

	free_ptr(keyboard);
	free_ptr(mouse);
	audio_player->stop(); // Stop all sounds and music so we don't free any that are playing.
	deinit_and_free_ptr(sound_cache);
	deinit_and_free_ptr(music_cache);
	deinit_and_free_ptr(audio_player);
	milk::graphics_quit();
	milk::window_quit();
	free_ptr(time);

	milk::State::initialized = false;
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