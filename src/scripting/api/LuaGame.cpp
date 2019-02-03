#include "LuaGame.h"

#include "SDL.h"

#include "game/Game.h"
#include "scene/Scene.h"

void milk::lua::game::loadScene(milk::Game& game, const std::string& sceneToLoad)
{
    game.loadScene(sceneToLoad);
}