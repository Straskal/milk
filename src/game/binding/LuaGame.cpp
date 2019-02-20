#include "LuaGame.h"

#include "scripting/sol/sol.hpp"

#include "game/Game.h"

void milk::lua::LuaGame::bind(sol::state& luaState)
{
	luaState.new_usertype<Game>("Game",
		"load_scene", &LuaGame::loadScene);

	luaState["Game"] = &Game::instance();
}

void milk::lua::LuaGame::loadScene(milk::Game& game, const std::string& sceneToLoad)
{
	game.loadScene(sceneToLoad);
}
