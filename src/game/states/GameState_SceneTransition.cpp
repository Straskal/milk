#include "GameState_SceneTransition.h"

#include "assetcache/AssetCache.h"

#include "game/Game.h"
#include "game/states/GameState_Scene.h"

#include "scene/Scene.h"

milk::GameState_SceneTransition::GameState_SceneTransition(milk::Game& game, const std::string& sceneToLoad)
        : GameState(game),
          sceneLoader_(game),
          sceneToLoad_(sceneToLoad)
{
}

std::unique_ptr<milk::GameState> milk::GameState_SceneTransition::checkState()
{
    auto scene = sceneLoader_.load(sceneToLoad_);

    game_.textureCache().freeUnreferencedAssets();

    return std::make_unique<GameState_Scene>(game_, std::move(scene));
}

bool milk::GameState_SceneTransition::transparent()
{
    return false;
}
