#include "LuaGame.h"

#include "SDL.h"

#include "game/Game.h"
#include "scene/Scene.h"

void milk::lua::game::loadScene(milk::Game& game, const std::string& sceneToLoad)
{
    game.loadScene(sceneToLoad);
}

milk::Scene& milk::lua::game::currentScene(milk::Game& game)
{
    // Shouldn't be null if it's being called from a Script.
    SDL_assert(game.currentScene() != nullptr);
    return *(game.currentScene());
}