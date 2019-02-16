#include "LuaApi.h"

#include "game/binding/LuaGame.h"
#include "input/binding/LuaKeyboard.h"
#include "math/binding/LuaMathf.h"
#include "math/binding/LuaVector2.h"
#include "physics/binding/LuaCollision.h"
#include "scene/binding/LuaActor.h"
#include "scene/binding/LuaScene.h"
#include "window/binding/LuaWindow.h"

void milk::LuaApi::init(sol::state& luaState)
{
    lua::LuaActor::bind(luaState);
    lua::LuaCollision::bind(luaState);
    lua::LuaKeyboard::bind(luaState);
    lua::LuaGame::bind(luaState);
    lua::LuaMathf::bind(luaState);
    lua::LuaScene::bind(luaState);
    lua::LuaVector2::bind(luaState);
    lua::LuaWindow::bind(luaState);
}
