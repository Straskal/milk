#include "Logic.h"

#include "Script.h"
#include "scene/Actor.h"
#include "events/GameEvents.h"

milk::Logic::Logic(sol::state& luaState)
        : luaState_(luaState)
{
}

void milk::Logic::handleEvent(GameEvent& gameEvent)
{
    switch (gameEvent.type())
    {
        case GameEventType::ACTOR_SPAWNED:
        {
            auto& spawnedEvent = dynamic_cast<ActorSpawnedEvent&>(gameEvent);
            onActorSpawned(spawnedEvent.actor());
        }
            break;
        case GameEventType::ACTOR_DETROYED:
        {
            auto& destroyedEvent = dynamic_cast<ActorDestroyedEvent&>(gameEvent);
            onActorDestroyed(destroyedEvent.actor());
        }
            break;
        case GameEventType::ACTOR_COLLISION:
        {
            auto& collisionEvent = dynamic_cast<ActorCollisionEvent&>(gameEvent);
            onActorCollision(collisionEvent);
        }
            break;
        case GameEventType::SCENE_CHANGED:
        {
            // TODO: should probably have a scne ending callback for scripts.
            scriptByActorId_.clear();
        }
            break;
        default:
            break;
    }
}

void milk::Logic::update()
{
    for (auto& it : scriptByActorId_)
        it.second->update();
}


void milk::Logic::lateUpdate()
{
    for (auto& it : scriptByActorId_)
        it.second->lateUpdate();
}

void milk::Logic::onActorSpawned(Actor& actor)
{
    auto script = actor.getComponent<Script>();

    if (script == nullptr)
        return;

    scriptByActorId_.insert(std::make_pair(actor.id(), script));

    script->load(luaState_);

    script->begin();
}

void milk::Logic::onActorDestroyed(Actor& actor)
{
    auto found = scriptByActorId_.find(actor.id());

    if (found == scriptByActorId_.end())
        return;

    auto script = found->second;

    if (script == nullptr)
        return;

    script->end();

    scriptByActorId_.erase(actor.id());
}

void milk::Logic::onActorCollision(ActorCollisionEvent& collisionEvent)
{
    auto script = scriptByActorId_.find(collisionEvent.actor().id());

    if (script != scriptByActorId_.end())
        script->second->onCollision(collisionEvent);
}