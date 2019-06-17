#include "MilkState.h"

#include <iostream>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "SDL.h"

#include "Locator.h"
#include "LuaApi.h"
#include "lua_extensions.h"

#include "audio/sdl/SDLSoundCache.h"
#include "audio/sdl/SDLAudioPlayer.h"
#include "input/sdl/SDLKeyboard.h"
#include "video/Color.h"
#include "video/sdl/SDLWindow.h"
#include "video/sdl/SDLRenderer.h"
#include "video/sdl/SDLTextureCache.h"

#define free_ptr(x) delete x; x = nullptr
#define deinit_and_free_ptr(x) x->free(); free_ptr(x)

static const int MILK_SUCCESS = 0;
static const int MILK_FAIL = 1;
static const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16

// The error handler and callbacks are not stored as lua references.
// They always just sit at the bottom of the stack.
static const int ERROR_HANDLER_STACK_INDEX = 1;
static const int CALLBACK_TABLE_STACK_INDEX = 2;

static int error_handler(lua_State* L) {
	luaL_traceback(L, L, lua_tostring(L, 1), 0);
	return 1;
}

static void print_runtime_error(const char* err) {
	std::cout << "RUNTIME ERROR: " << err << std::endl << std::endl;
	std::cout << "Press enter to continue execution..." << std::endl;
	int _ = std::getchar(); // Wait for user input before continuing execution
}

milk::MilkState::MilkState()
	: m_lua{ nullptr }
	, m_window{ nullptr }
	, m_renderer{ nullptr }
	, m_keyboard{ nullptr }
	, m_textures{ nullptr }
	, m_audioPlayer{ nullptr }
	, m_audioCache{ nullptr } { }

int milk::MilkState::run(const std::string& configPath) {
	m_window = new SDLWindow();
	if (!m_window->init()) {
		deinit_and_free_ptr(m_window);
		return MILK_FAIL;
	}

	m_renderer = new SDLRenderer();
	if (!m_renderer->init(m_window->handle())) {
		deinit_and_free_ptr(m_renderer);
		deinit_and_free_ptr(m_window);
		return MILK_FAIL;
	}

	m_textures = new SDLTextureCache();
	if (!m_textures->init(m_renderer->handle())) {
		deinit_and_free_ptr(m_renderer);
		deinit_and_free_ptr(m_window);
		deinit_and_free_ptr(m_textures);
		return MILK_FAIL;
	}

	m_audioPlayer = new SDLAudioPlayer();
	if (!m_audioPlayer->init()) {
		deinit_and_free_ptr(m_audioPlayer);
		deinit_and_free_ptr(m_renderer);
		deinit_and_free_ptr(m_window);
		deinit_and_free_ptr(m_textures);
		return MILK_FAIL;
	}

	m_audioCache = new SDLSoundCache();
	m_keyboard = new SDLKeyboard();

	// 'Register' systems with the service locator for lua modules
	Locator::window = m_window;
	Locator::renderer = m_renderer;
	Locator::keyboard = m_keyboard;
	Locator::textures = m_textures;
	Locator::sounds = m_audioCache;
	Locator::audioPlayer = m_audioPlayer;

	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);
	LuaApi::open(m_lua);

	// Our error handler is going to the #1 on the stack
	lua_pushcfunction(m_lua, error_handler);

	if (luaL_loadfile(m_lua, "res/main.lua") != LUA_OK || lua_pcall(m_lua, 0, 1, ERROR_HANDLER_STACK_INDEX) != LUA_OK) {
		const char* stacktrace = lua_tostring(m_lua, -1);
		print_runtime_error(stacktrace);

		lua_close(m_lua);
		free_ptr(m_keyboard);
		deinit_and_free_ptr(m_audioCache);
		deinit_and_free_ptr(m_audioPlayer);
		deinit_and_free_ptr(m_renderer);
		deinit_and_free_ptr(m_window);
		deinit_and_free_ptr(m_textures);
		return MILK_FAIL;
	}

	// If main.lua fails or does not return a single table, then frig off.
	if (!lua_istable(m_lua, -1)) {
		print_runtime_error("main.lua must return a table containing callback functions.");

		lua_close(m_lua);
		free_ptr(m_keyboard);
		deinit_and_free_ptr(m_audioCache);
		deinit_and_free_ptr(m_audioPlayer);
		deinit_and_free_ptr(m_renderer);
		deinit_and_free_ptr(m_window);
		deinit_and_free_ptr(m_textures);
		return MILK_FAIL;
	}

	// Show the window AFTER running main.lua
	// This gives main.lua a chance to customize the window, renderer, etc...
	m_window->show();

	while (!m_window->shouldClose()) {
		int frameStart = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				m_window->close();
			}
		}

		m_keyboard->updateState();

		lua_getfield(m_lua, CALLBACK_TABLE_STACK_INDEX, "tick");
		if (lua_pcall(m_lua, 0, 0, ERROR_HANDLER_STACK_INDEX) != LUA_OK) {
			const char* stacktrace = lua_tostring(m_lua, -1);
			lua_pop(m_lua, 1); // Pop stacktrace from stack
			print_runtime_error(stacktrace);
		}

		m_renderer->clear();

		lua_getfield(m_lua, CALLBACK_TABLE_STACK_INDEX, "render");
		if (lua_pcall(m_lua, 0, 0, ERROR_HANDLER_STACK_INDEX) != LUA_OK) {
			const char* stacktrace = lua_tostring(m_lua, -1);
			lua_pop(m_lua, 1); // Pop stacktrace from stack
			print_runtime_error(stacktrace);
		}

		m_renderer->present();

		// As of right now, milk runs at a fixed timestep of 16 milliseconds.
		// The only time that this becomes a problem is if the game can't run at 60 FPS.
		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < MILLISECONDS_PER_FRAME) {
			SDL_Delay((Uint32)(MILLISECONDS_PER_FRAME - frameTime));
		}
	}

	lua_close(m_lua);
	free_ptr(m_keyboard);
	deinit_and_free_ptr(m_audioCache);
	deinit_and_free_ptr(m_audioPlayer);
	deinit_and_free_ptr(m_renderer);
	deinit_and_free_ptr(m_window);
	deinit_and_free_ptr(m_textures);
	return MILK_SUCCESS;
}
