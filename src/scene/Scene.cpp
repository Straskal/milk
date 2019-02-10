#include "Scene.h"

#include <memory>

#include "Actor.h"
#include "ActorComponent.h"
#include "asset/ActorLoader.h"

#include "graphics/Tilemap.h"

#include "window/Window.h"

milk::Scene::Scene(const std::string& name, ActorLoader& actorLoader)
        : name_(name),
          actorLoader_(actorLoader),
          tilemap_(nullptr),
          ended_(false)
{
}

milk::Scene::~Scene() = default;

milk::Actor* milk::Scene::spawnActor(const std::string& actorName, milk::Vector2 position)
{
    int id = idGenerator_.popId();

    auto actor = std::make_unique<Actor>(*this, id, actorName, position);

    auto pActor = actor.get();

    if (ended_)
        return pActor;

    actorsToSpawn_.emplace_back(std::move(actor));

    return pActor;
}

milk::Actor* milk::Scene::spawnActor(const std::string& actorName, Vector2 position, const std::string& templateName)
{
    auto pActor = spawnActor(actorName, position);

    actorLoader_.load(*pActor, templateName);

    return pActor;
}

bool milk::Scene::destroyActor(int id)
{
    if (actorsById_.find(id) == actorsById_.end())
        return false;

    actorsToDestroy_.emplace_back(id);

    return true;
}

milk::Actor* milk::Scene::findActor(const std::string& name) const
{
    // TODO: Find a better solution than brute force implementation.
    for (auto& itr : actorsToSpawn_)
    {
        if (itr->name() == name)
            return itr.get();
    }

    for (auto& it : actorsById_)
    {
        if (it.second->name() == name)
            return it.second.get();
    }

    return nullptr;
}

milk::Camera& milk::Scene::camera()
{
    return camera_;
}

milk::Tilemap* milk::Scene::tilemap()
{
    return tilemap_.get();
}

void milk::Scene::tilemap(std::unique_ptr<milk::Tilemap> tilemap)
{
    tilemap_ = std::move(tilemap);
}

milk::Rectangle milk::Scene::bounds() const
{
    return Rectangle{0, 0, tilemap_->width, tilemap_->height};
}

void milk::Scene::end()
{
    ended_ = true;
}

milk::Actor* milk::Scene::pollSpawned()
{
    if (actorsToSpawn_.empty())
        return nullptr;

    auto& pSpawned = actorsToSpawn_.back();

    auto pSpawnedRaw = pSpawned.get();

    actorsById_.insert(std::make_pair(pSpawned->id(), std::move(pSpawned)));

    actorsToSpawn_.pop_back();

    return pSpawnedRaw;
}

milk::Actor* milk::Scene::pollDestroyed()
{
    static int lastPolledId = -1;

    if (actorsToDestroy_.empty())
    {
        lastPolledId = -1;
        return nullptr;
    }

    if (lastPolledId > -1)
    {
        actorsById_.erase(lastPolledId);
        idGenerator_.pushId(lastPolledId);
    }

    int destroyedId = actorsToDestroy_.back();

    lastPolledId = destroyedId;

    actorsToDestroy_.pop_back();

    return actorsById_.at(destroyedId).get();
}
