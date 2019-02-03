#include "GameState_Scene.h"

#include "debugtools/DebugTools.h"

#include "game/Game.h"
#include "game/states/GameState.h"
#include "game/states/GameState_SceneTransition.h"

#include "graphics/Graphics.h"

#include "physics/Physics.h"

#include "scene/Scene.h"
#include "scripting/Logic.h"

#include "window/adapter/RendererAdapter.h"
#include "window/adapter/WindowAdapter.h"

milk::GameState_Scene::GameState_Scene(milk::Game& game, milk::Scene& scene)
        : GameState(game),
          scene_(scene)
{
}

std::unique_ptr<milk::GameState> milk::GameState_Scene::checkState()
{
    return game_.sceneToLoad_.empty() ? nullptr : std::make_unique<GameState_SceneTransition>(game_);
}

void milk::GameState_Scene::update()
{
    // Lets handle all of the actors that were spawned last frame!
    while (auto spawned = scene_.pollSpawned())
    {
        game_.physics_->onActorSpawned(*spawned);
        game_.graphics_->onActorSpawned(*spawned);
#if _DEBUG
        game_.debugTools_->onActorSpawned(*spawned);
#endif
        game_.logic_->onActorSpawned(*spawned);
    }

    // Now lets all of the actors that were destroyed last frame!
    while (auto destroyed = scene_.pollDestroyed())
    {
        game_.physics_->onActorDestroyed(*destroyed);
        game_.graphics_->onActorDestroyed(*destroyed);
#if _DEBUG
        game_.debugTools_->onActorDestroyed(*destroyed);
#endif
        game_.logic_->onActorDestroyed(*destroyed);
    }

    // NOW lets handle all of the collisions last frame!
    while (auto collisionEvent = game_.physics_->pollCollisions())
    {
        game_.logic_->onActorCollision(*collisionEvent);
    }

    game_.logic_->update();
    game_.physics_->update();
    game_.logic_->lateUpdate();
}

void milk::GameState_Scene::render()
{
    game_.graphics_->render(scene_);
#if _DEBUG
    game_.debugTools_->render(scene_);
#endif
}

void milk::GameState_Scene::end()
{
    scene_.end();

    game_.logic_->flush();
    game_.physics_->flush();
    game_.graphics_->flush();
#if _DEBUG
    game_.debugTools_->flush();
#endif
}

bool milk::GameState_Scene::transparent()
{
    return false;
}
