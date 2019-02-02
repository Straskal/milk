#include "SceneManager.h"

#include "scene/Scene.h"

#include "events/EventQueue.h"
#include "events/GameEvents.h"

#include "SceneLoader.h"

milk::SceneManager::SceneManager(EventQueue& eventQueue, SceneLoader& sceneLoader)
        : eventQueue_(eventQueue),
          sceneLoader_(sceneLoader),
          scene_(nullptr)
{
}

milk::SceneManager::~SceneManager() = default;

void milk::SceneManager::loadScene(const std::string& scene)
{
    // Will generate actor destroyed events for ALL actors.
    if (scene_ != nullptr)
        scene_->end();

    // Push scene changed event so all systems can do their thing.
    eventQueue_.pushEvent<SceneChangedEvent>(scene);

    sceneToLoad_ = scene;
}

void milk::SceneManager::update()
{
    if (!sceneToLoad_.empty())
    {
        // By this time, all systems should have already handled the scene change.
        // So lets free the scene from mem and load the next one.
        if (scene_ != nullptr)
            scene_.reset();

        if (sceneToLoad_ == MILK_NULL_SCENE)
        {
            sceneToLoad_.erase();
            return;
        }

        scene_ = sceneLoader_.load(sceneToLoad_);

        eventQueue_.pushEvent<SceneLoadedEvent>(sceneToLoad_);

        sceneToLoad_.erase();
    }

    if (scene_ != nullptr)
        scene_->syncActorLists();
}

milk::Scene* milk::SceneManager::currentScene() const
{
    return scene_.get();
}
