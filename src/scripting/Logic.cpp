#include "Logic.h"

#include "Script.h"

#include "physics/BoxCollider.h"
#include "physics/Collision.h"

#include "scene/Actor.h"

milk::Logic::Logic(sol::state& luaState)
        : luaState_(luaState)
{
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

void milk::Logic::onActorCollision(Collision& collision)
{
    auto script = scriptByActorId_.find(collision.actor.actor().id());

    if (script != scriptByActorId_.end())
        script->second->onCollision(collision);
}

void milk::Logic::flush()
{
    for (auto& scriptItr : scriptByActorId_)
        scriptItr.second->end();

    scriptByActorId_.clear();
}
