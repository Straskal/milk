#include "MilkState.h"

#include "SDL.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "Locator.h"
#include "input/sdl/Keyboard.h"
#include "lua/lua_extensions.h"
#include "lua/api/LuaApi.h"
#include "graphics/Color.h"
#include "window/sdl/Window.h"
#include "window/sdl/Renderer.h"

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
	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);

	luaL_dofile(m_lua, configPath.c_str());

	lua_getfield(m_lua, -1, "window");
	std::string winTitle = lua::get_string_field(m_lua, "title");
	int winHeight = lua::get_int_field(m_lua, "height");
	int winWidth = lua::get_int_field(m_lua, "width");
	bool winFullscreen = lua::get_bool_field(m_lua, "fullscreen");
	lua_pop(m_lua, -2);

	m_window = new sdl::Window();
	if (!m_window->init(winTitle, winWidth, winHeight, winFullscreen)) {
		m_window->free(); delete m_window; m_window = nullptr;
		lua_close(m_lua);
		return MILK_FAIL;
	}

	lua_getfield(m_lua, -1, "renderer");
	int vwidth = lua::get_int_field(m_lua, "vwidth");
	int vheight = lua::get_int_field(m_lua, "vheight");
	// Pop the entire config table off of the stack
	lua_pop(m_lua, 1);

	m_renderer = new sdl::Renderer();
	if (!m_renderer->init(m_window->handle(), vwidth, vheight)) {
		m_renderer->free(); delete m_renderer; m_renderer = nullptr;
		m_window->free(); delete m_window; m_window = nullptr;
		lua_close(m_lua);
		return MILK_FAIL;
	}

	m_keyboard = new sdl::Keyboard();
	m_keyboard->init();

	// 'Register' systems with the service locator
	Locator::window = m_window;
	Locator::renderer = m_renderer;
	Locator::keyboard = m_keyboard;

	LuaApi::registerApi(m_lua);

	luaL_dofile(m_lua, "res/main.lua");
	int maintable = luaL_ref(m_lua, LUA_REGISTRYINDEX);

	while (m_running) {
		int frameStartTime = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				m_running = false;
			}
		}

		m_keyboard->updateState();

		lua_rawgeti(m_lua, LUA_REGISTRYINDEX, maintable);
		lua_getfield(m_lua, -1, "tick");
		lua_pcall(m_lua, 0, 0, NULL);

		m_renderer->clear(Color::black());
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
