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

milk::GameState_Scene::GameState_Scene(milk::Game& game, std::unique_ptr<milk::Scene> scene)
        : GameState(game),
          scene_(std::move(scene)),
          logic_(nullptr),
          physics_(nullptr),
          graphics_(nullptr)
{
}

milk::GameState_Scene::~GameState_Scene() = default;

void milk::GameState_Scene::begin()
{
    logic_ = std::make_unique<Logic>(game_.luaState());
    physics_ = std::make_unique<Physics>();
    graphics_ = std::make_unique<Graphics>(game_.window().renderer(), game_.textureCache());
}

std::unique_ptr<milk::GameState> milk::GameState_Scene::checkState()
{
    return sceneToLoad_.empty() ? nullptr : std::make_unique<GameState_SceneTransition>(game_, sceneToLoad_);
}

void milk::GameState_Scene::update()
{
    // Lets handle all of the actors that were spawned last frame!
    while (auto spawned = scene_->pollSpawned())
    {
        physics_->onActorSpawned(*spawned);
        graphics_->onActorSpawned(*spawned);
#if _DEBUG
        game_.debugTools().onActorSpawned(*spawned);
#endif
        logic_->onActorSpawned(*spawned);
    }

    // Now lets all of the actors that were destroyed last frame!
    while (auto destroyed = scene_->pollDestroyed())
    {
        physics_->onActorDestroyed(*destroyed);
        graphics_->onActorDestroyed(*destroyed);
#if _DEBUG
        game_.debugTools().onActorDestroyed(*destroyed);
#endif
        logic_->onActorDestroyed(*destroyed);
    }

    // NOW lets handle all of the collisions last frame!
    while (auto collisionEvent = physics_->pollCollisions())
    {
        logic_->onActorCollision(*collisionEvent);
    }

    logic_->update();
    physics_->update();
    logic_->lateUpdate();
}

void milk::GameState_Scene::render()
{
    graphics_->render(*scene_);
#if _DEBUG
    game_.debugTools().render(*scene_);
#endif
}

void milk::GameState_Scene::end()
{
    scene_->end();

    logic_->flush();
    physics_->flush();
    graphics_->flush();
#if _DEBUG
    game_.debugTools().flush();
#endif
}

bool milk::GameState_Scene::transparent()
{
    return false;
}

void milk::GameState_Scene::loadScene(const std::string& sceneToLoad)
{
    sceneToLoad_ = sceneToLoad;
}
