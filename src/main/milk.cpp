#include "milk.h"

#include "graphics/Color.h"
#include "lua/LuaEnvironment.h"
#include "window/Window.h"
#include "window/Renderer.h"

bool milk::state::init(milk::MilkState& milkState) {
	milkState.luaenv->init(&milkState);
	MilkStartupConfig config = milkState.luaenv->getConfiguration("res/config.lua");

	if (!milkState.window->init(config.winTitle, config.winWidth, config.winHeight, config.winFullscreen)) {
		milkState.window->free();
		return false;
	}

	if (!milkState.renderer->init(milkState.window->handle(), config.resWidth, config.resHeight)) {
		milkState.window->free();
		milkState.renderer->free();
		return false;
	}

	milkState.running = true;
}

void milk::state::tick(milk::MilkState& milkState) {
	milkState.luaenv->tick();
}

void milk::state::postTick(milk::MilkState& milkState) {
	milkState.luaenv->postTick();
}

void milk::state::render(milk::MilkState& milkState) {
	milkState.renderer->clear(Color::black());
	milkState.luaenv->render();
	milkState.renderer->present();
}

void milk::state::postRender(milk::MilkState& milkState) {
	milkState.luaenv->postRender();
}

void milk::state::quit(milk::MilkState& milkState) {
	milkState.renderer->free();
	milkState.window->free();
	milkState.luaenv->free();
}