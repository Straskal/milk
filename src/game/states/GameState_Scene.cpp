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

void milk::GameState_Scene::begin()
{
    while (auto spawned = scene_.pollSpawned())
    {
        game_.physics_->onActorSpawned(*spawned);
        game_.graphics_->onActorSpawned(*spawned);
        game_.debugTools_->onActorSpawned(*spawned);
        game_.logic_->onActorSpawned(*spawned);
    }

    while (auto destroyed = scene_.pollDestroyed())
    {
        game_.physics_->onActorDestroyed(*destroyed);
        game_.graphics_->onActorDestroyed(*destroyed);
        game_.debugTools_->onActorDestroyed(*destroyed);
        game_.logic_->onActorDestroyed(*destroyed);
    }

    game_.logic_->update();
    game_.physics_->update();
    game_.logic_->lateUpdate();
}

void milk::GameState_Scene::handleEvent(GameEvent& gameEvent)
{
    game_.physics_->handleEvent(gameEvent);
    game_.graphics_->handleEvent(gameEvent);
    game_.debugTools_->handleEvent(gameEvent);
    game_.logic_->handleEvent(gameEvent);
}

std::unique_ptr<milk::GameState> milk::GameState_Scene::update()
{
    if (!game_.sceneToLoad_.empty())
        return std::make_unique<GameState_SceneTransition>(game_);

    while (auto spawned = scene_.pollSpawned())
    {
        game_.physics_->onActorSpawned(*spawned);
        game_.graphics_->onActorSpawned(*spawned);
        game_.debugTools_->onActorSpawned(*spawned);
        game_.logic_->onActorSpawned(*spawned);
    }

    while (auto destroyed = scene_.pollDestroyed())
    {
        game_.physics_->onActorDestroyed(*destroyed);
        game_.graphics_->onActorDestroyed(*destroyed);
        game_.debugTools_->onActorDestroyed(*destroyed);
        game_.logic_->onActorDestroyed(*destroyed);
    }

    game_.logic_->update();
    game_.physics_->update();
    game_.logic_->lateUpdate();

    return nullptr;
}

void milk::GameState_Scene::render()
{
    game_.graphics_->render(scene_);
    game_.debugTools_->render(scene_);
}

void milk::GameState_Scene::end()
{
    scene_.end();

    game_.logic_->flush();
    game_.physics_->flush();
    game_.graphics_->flush();
    game_.debugTools_->flush();
}

bool milk::GameState_Scene::transparent()
{
    return false;
}
