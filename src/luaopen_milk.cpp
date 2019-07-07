#include "luaopen_milk.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "SDL.h"

#include "Locator.h"
#include "luamlib.h"

#include "audio/sdl/SDLAudioPlayer.h"
#include "audio/sdl/SDLMusicCache.h"
#include "audio/sdl/SDLSoundCache.h"
#include "graphics/sdl/SDLRenderer.h"
#include "graphics/sdl/SDLImageCache.h"
#include "keyboard/sdl/SDLKeyboard.h"
#include "mouse/sdl/SDLMouse.h"
#include "time/sdl/SDLTime.h"
#include "window/sdl/SDLWindow.h"

#define free_ptr(x) delete x; x = nullptr
#define deinit_and_free_ptr(x) x->free(); free_ptr(x)

static milk::SDLTime* time = nullptr;
static milk::SDLWindow* window = nullptr;
static milk::SDLRenderer* renderer = nullptr;
static milk::SDLMouse* mouse = nullptr;
static milk::SDLKeyboard* keyboard = nullptr;
static milk::SDLImageCache* image_cache = nullptr;
static milk::SDLAudioPlayer* audio_player = nullptr;
static milk::SDLSoundCache* sound_cache = nullptr;
static milk::SDLMusicCache* music_cache = nullptr;

static int milk_init(lua_State* L)
{
	if (milk::State::initialized) {
		return luaL_error(L, "milk has already been initialized!");
	}

	time = new milk::SDLTime();
	window = new milk::SDLWindow();
	renderer = new milk::SDLRenderer();
	image_cache = new milk::SDLImageCache();
	audio_player = new milk::SDLAudioPlayer();
	music_cache = new milk::SDLMusicCache();
	sound_cache = new milk::SDLSoundCache();
	mouse = new milk::SDLMouse();
	keyboard = new milk::SDLKeyboard();

	milk::State::time = time;
	milk::State::window = window;
	milk::State::renderer = renderer;
	milk::State::images = image_cache;
	milk::State::audioPlayer = audio_player;
	milk::State::sounds = sound_cache;
	milk::State::music = music_cache;
	milk::State::mouse = mouse;
	milk::State::keyboard = keyboard;

	if (window->init()
		&& renderer->init(window->handle())
		&& image_cache->init(renderer->handle())
		&& audio_player->init()) {

		time->start();
		milk::State::initialized = true;
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}
	return 1;
}

static int milk_poll(lua_State* L)
{
	mouse->reset();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			window->close();
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
	deinit_and_free_ptr(image_cache);
	deinit_and_free_ptr(renderer);
	deinit_and_free_ptr(window);
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