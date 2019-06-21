#include "run.h"

#include <iostream>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "SDL.h"

#include "Locator.h"
#include "luamlib.h"

#include "audio/sdl/SDLAudioPlayer.h"
#include "audio/sdl/SDLMusicCache.h"
#include "audio/sdl/SDLSoundCache.h"
#include "graphics/Color.h"
#include "graphics/sdl/SDLRenderer.h"
#include "graphics/sdl/SDLTextureCache.h"
#include "keyboard/sdl/SDLKeyboard.h"
#include "mouse/sdl/SDLMouse.h"
#include "window/sdl/SDLWindow.h"

#ifndef MILK_MAIN_LUA_PATH
#define MILK_MAIN_LUA_PATH "main.lua"
#endif

#define free_ptr(x) delete x; x = nullptr
#define deinit_and_free_ptr(x) x->free(); free_ptr(x)

static const int MILK_SUCCESS = 0;
static const int MILK_FAIL = 1;
static const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16
static const int ERROR_HANDLER_STACK_INDEX = 1;
static const int CALLBACK_TABLE_STACK_INDEX = 2;

static milk::SDLWindow* window = nullptr;
static milk::SDLRenderer* renderer = nullptr;
static milk::SDLMouse* mouse = nullptr;
static milk::SDLKeyboard* keyboard = nullptr;
static milk::SDLTextureCache* texture_cache = nullptr;
static milk::SDLAudioPlayer* audio_player = nullptr;
static milk::SDLSoundCache* sound_cache = nullptr;
static milk::SDLMusicCache* music_cache = nullptr;
static lua_State* lua = nullptr;

static int error_handler(lua_State* L) {
	luaL_traceback(L, L, lua_tostring(L, 1), 0);
	return 1;
}

static void print_runtime_error(const char* err) {
	std::cout << "RUNTIME ERROR: " << err << std::endl << std::endl;
	std::cout << "Press enter to continue execution..." << std::endl;
	int _ = std::getchar(); // Wait for user input before continuing execution
}

static bool init() {
	window = new milk::SDLWindow();
	renderer = new milk::SDLRenderer();
	texture_cache = new milk::SDLTextureCache();
	audio_player = new milk::SDLAudioPlayer();
	music_cache = new milk::SDLMusicCache();
	sound_cache = new milk::SDLSoundCache();
	mouse = new milk::SDLMouse();
	keyboard = new milk::SDLKeyboard();

	return window->init() // SDL_Init
		&& renderer->init(window->handle())
		&& texture_cache->init(renderer->handle()) // IMG_Init
		&& audio_player->init(); // Mix_init
}

// 'Register' systems with the service locator for lua modules
static void register_locator() {
	milk::Locator::window = window;
	milk::Locator::renderer = renderer;
	milk::Locator::textures = texture_cache;
	milk::Locator::audioPlayer = audio_player;
	milk::Locator::sounds = sound_cache;
	milk::Locator::music = music_cache;
	milk::Locator::mouse = mouse;
	milk::Locator::keyboard = keyboard;
}

static bool init_api_and_callbacks() {
	lua = luaL_newstate();
	luaL_openlibs(lua);

	// Open all milk libs
	milk::luaM_openlibs(lua);

	// Our error handler is going to be #1 on the stack
	lua_pushcfunction(lua, error_handler);

	if (luaL_loadfile(lua, MILK_MAIN_LUA_PATH) != LUA_OK || lua_pcall(lua, 0, 1, ERROR_HANDLER_STACK_INDEX) != LUA_OK) {
		const char* stacktrace = lua_tostring(lua, -1);
		print_runtime_error(stacktrace);
		return false;
	}

	// If main.lua fails or does not return a single table, then frig off.
	if (!lua_istable(lua, -1)) {
		print_runtime_error("main.lua must return a table containing callback functions.");
		return false;
	}
}

static void main_loop() {
	window->show();

	while (!window->shouldClose()) {
		int frameStart = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				window->close();
			}
		}

		mouse->updateState();
		keyboard->updateState();

		lua_getfield(lua, CALLBACK_TABLE_STACK_INDEX, "tick");
		if (lua_pcall(lua, 0, 0, ERROR_HANDLER_STACK_INDEX) != LUA_OK) {
			const char* stacktrace = lua_tostring(lua, -1);
			lua_pop(lua, 1); // Pop stacktrace from stack
			print_runtime_error(stacktrace);
		}

		renderer->clear();

		lua_getfield(lua, CALLBACK_TABLE_STACK_INDEX, "draw");
		if (lua_pcall(lua, 0, 0, ERROR_HANDLER_STACK_INDEX) != LUA_OK) {
			const char* stacktrace = lua_tostring(lua, -1);
			lua_pop(lua, 1); // Pop stacktrace from stack
			print_runtime_error(stacktrace);
		}

		renderer->present();

		// As of right now, milk runs at a fixed timestep of 16 milliseconds.
		// The only time that this becomes a problem is if the game can't run at 60 FPS.
		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < MILLISECONDS_PER_FRAME) {
			SDL_Delay((Uint32)(MILLISECONDS_PER_FRAME - frameTime));
		}
	}
}

static void deinit() {
	lua_close(lua);
	free_ptr(keyboard);
	free_ptr(mouse);
	deinit_and_free_ptr(sound_cache);
	deinit_and_free_ptr(music_cache);
	deinit_and_free_ptr(audio_player);
	deinit_and_free_ptr(texture_cache);
	deinit_and_free_ptr(renderer);
	deinit_and_free_ptr(window);
}

int milk::run() {
	if (!init()) {
		deinit();
		return MILK_FAIL;
	}

	register_locator();

	if (!init_api_and_callbacks()) {
		deinit();
		return MILK_FAIL;
	}

	main_loop();
	deinit();
	return MILK_SUCCESS;
}
