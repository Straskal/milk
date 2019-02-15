#include "LuaApi.h"

#include "scripting/bindings/LuaActor.h"
#include "scripting/bindings/LuaCollision.h"
#include "scripting/bindings/LuaKeyboard.h"
#include "scripting/bindings/LuaGame.h"
#include "scripting/bindings/LuaMathf.h"
#include "scripting/bindings/LuaScene.h"
#include "scripting/bindings/LuaVector2.h"
#include "scripting/bindings/LuaWindow.h"

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
