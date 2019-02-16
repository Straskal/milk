#include "LuaWindow.h"

#include "sol/sol.hpp"

#include "game/Game.h"

#include "window/Window.h"

void milk::lua::LuaWindow::bind(sol::state& luaState)
{
    luaState.new_usertype<Window>("Window",
                                  "width", &LuaWindow::width,
                                  "height", &LuaWindow::height,
                                  "fullscreen", &LuaWindow::fullscreen,
                                  "toggle_fullscreen", &LuaWindow::toggleFullscreen);

    luaState["Window"] = &Game::instance().window();
}

int milk::lua::LuaWindow::width(milk::Window& window)
{
    return window.width();
}

int milk::lua::LuaWindow::height(milk::Window& window)
{
    return window.height();
}

bool milk::lua::LuaWindow::fullscreen(milk::Window& window)
{
    return window.fullscreen();
}

void milk::lua::LuaWindow::toggleFullscreen(milk::Window& window)
{
    window.toggleFullscreen();
}
