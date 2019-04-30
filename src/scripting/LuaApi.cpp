#include "LuaApi.h"

#include "game/binding/LuaGame.h"
#include "input/binding/LuaKeyboard.h"
#include "math/binding/LuaMathf.h"
#include "math/binding/LuaVector2.h"
#include "window/binding/LuaWindow.h"

void milk::LuaApi::init(sol::state& luaState)
{
    lua::LuaKeyboard::bind(luaState);
    lua::LuaGame::bind(luaState);
    lua::LuaMathf::bind(luaState);
    lua::LuaVector2::bind(luaState);
    lua::LuaWindow::bind(luaState);
}
