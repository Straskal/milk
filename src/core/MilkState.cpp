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

static const int MILK_SUCCESS = 0;
static const int MILK_FAIL = 1;

static const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16

milk::MilkState::MilkState()
	: m_lua{ nullptr }
	, m_window{ nullptr }
	, m_renderer{ nullptr }
	, m_keyboard{ nullptr }
	, m_running{ true }{ }

int milk::MilkState::run(const std::string& configPath) {
	m_window = new SDLWindow();
	if (!m_window->init()) {
		m_window->free(); delete m_window; m_window = nullptr;
		return MILK_FAIL;
	}

	m_renderer = new SDLRenderer();
	if (!m_renderer->init(m_window->handle())) {
		m_renderer->free(); delete m_renderer; m_renderer = nullptr;
		m_window->free(); delete m_window; m_window = nullptr;
		return MILK_FAIL;
	}

	m_keyboard = new SDLKeyboard();

	// 'Register' systems with the service locator for lua modules
	Locator::window = m_window;
	Locator::renderer = m_renderer;
	Locator::keyboard = m_keyboard;

	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);

	LuaApi::open(m_lua);

	luaL_dofile(m_lua, "res/main.lua");
	int callbacks = luaL_ref(m_lua, LUA_REGISTRYINDEX);

	m_window->show();

	while (!m_window->shouldClose()) {
		int frameStartTime = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				m_running = false;
			}
		}

		m_keyboard->updateState();

		lua_rawgeti(m_lua, LUA_REGISTRYINDEX, callbacks);
		lua_getfield(m_lua, -1, "tick");
		if (lua_pcall(m_lua, 0, 0, NULL) != LUA_OK) {
			const char* err = lua_tostring(m_lua, -1);
			std::cout << err << std::endl;
		}
		
		m_renderer->clear(Color::black());
		lua_rawgeti(m_lua, LUA_REGISTRYINDEX, callbacks);
		lua_getfield(m_lua, -1, "render");
		if (lua_pcall(m_lua, 0, 0, NULL) != LUA_OK) {
			const char* err = lua_tostring(m_lua, -1);
			std::cout << err << std::endl;
		}
		m_renderer->present();

		Uint32 frameTime = SDL_GetTicks() - frameStartTime;
		if (frameTime < MILLISECONDS_PER_FRAME) {
			SDL_Delay((Uint32)(MILLISECONDS_PER_FRAME - frameTime));
		}
	}

	m_renderer->free(); delete m_renderer; m_renderer = nullptr;
	m_window->free(); delete m_window; m_window = nullptr;
	delete m_keyboard; m_keyboard = nullptr;
	lua_close(m_lua);

	return MILK_SUCCESS;
}
