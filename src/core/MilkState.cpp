#include "MilkState.h"

#include <iostream>

#include "SDL.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "Locator.h"
#include "LuaApi.h"
#include "lua_extensions.h"

#include "input/sdl/SDLKeyboard.h"
#include "video/Color.h"
#include "video/sdl/SDLWindow.h"
#include "video/sdl/SDLRenderer.h"
#include "video/sdl/SDLTextureCache.h"

static const int MILK_SUCCESS = 0;
static const int MILK_FAIL = 1;
static const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16

#define free_ptr(x) delete x; x = nullptr
#define deinit_and_free_ptr(x) x->free(); free_ptr(x)

milk::MilkState::MilkState()
	: m_lua{ nullptr }
	, m_window{ nullptr }
	, m_renderer{ nullptr }
	, m_keyboard{ nullptr }
	, m_textures{ nullptr } {}

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

	m_keyboard = new SDLKeyboard();

	// 'Register' systems with the service locator for lua modules
	Locator::window = m_window;
	Locator::renderer = m_renderer;
	Locator::keyboard = m_keyboard;
	Locator::textures = m_textures;

	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);
	LuaApi::open(m_lua);

	// Call do file and push callback table onto stack
	// If main.lua did not return a single table, then frig off.
	luaL_dofile(m_lua, "res/main.lua");
	if (lua_gettop(m_lua) != 1 || !lua_istable(m_lua, -1)) {
		lua_close(m_lua);
		free_ptr(m_keyboard);
		deinit_and_free_ptr(m_renderer);
		deinit_and_free_ptr(m_window);
		deinit_and_free_ptr(m_textures);
		std::cout << "ERROR: main.lua must return a single table containing callback functions" << std::endl;
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

		// Callback table is top of stack
		luaM::invoke_method(m_lua, "tick");

		m_renderer->clear();
		// Callback table is top of stack
		luaM::invoke_method(m_lua, "render");
		m_renderer->present();

		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < MILLISECONDS_PER_FRAME) {
			SDL_Delay((Uint32)(MILLISECONDS_PER_FRAME - frameTime));
		}
	}

	lua_close(m_lua);
	free_ptr(m_keyboard);
	deinit_and_free_ptr(m_renderer);
	deinit_and_free_ptr(m_window);
	deinit_and_free_ptr(m_textures);
	return MILK_SUCCESS;
}
