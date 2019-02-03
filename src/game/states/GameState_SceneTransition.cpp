#include "GameState_SceneTransition.h"

#include "assetcache/adapter/TextureCacheAdapter.h"
#include "game/Game.h"
#include "game/states/GameState_Scene.h"
#include "scene/Scene.h"

milk::GameState_SceneTransition::GameState_SceneTransition(milk::Game& game)
        : GameState(game),
          sceneLoader_(game)
{
}

void milk::GameState_SceneTransition::begin()
{
    game_.currentScene_.reset();

    if (game_.sceneToLoad_ != NULL_SCENE)
        game_.currentScene_ = sceneLoader_.load(game_.sceneToLoad_);
    else
        game_.isRunning_ = false;

    game_.sceneToLoad_.erase();

    game_.textureCache_->freeUnreferencedAssets();
}

std::unique_ptr<milk::GameState> milk::GameState_SceneTransition::checkState()
{
    return std::make_unique<GameState_Scene>(game_, *game_.currentScene_);
}

bool milk::GameState_SceneTransition::transparent()
{
    return false;
}
