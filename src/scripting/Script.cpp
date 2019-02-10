#include "Script.h"

#include "physics/Collision.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

const milk::ComponentType milk::Script::type = SCRIPT;

milk::Script::Script(Actor& actor, const std::string& scriptName)
        : ActorComponent(actor),
          scriptName_(scriptName)
{
}

void milk::Script::load(sol::state& luaState)
{
    luaScript_ = luaState.script_file(scriptName_);

    // Set "self.actor"
    // Set "self.scene"
    luaScript_["actor"] = &actor_;
    luaScript_["scene"] = &actor_.scene();
}

void milk::Script::begin()
{
    sol::function onBegin = luaScript_[SCRIPT_CALLBACK_BEGIN];

    if (!onBegin.valid())
        return;

    sol::table self = luaScript_;
    luaScript_[SCRIPT_CALLBACK_BEGIN](self);
}

void milk::Script::update()
{
    sol::function onUpdate = luaScript_[SCRIPT_CALLBACK_UPDATE];

    if (!onUpdate.valid())
        return;

    sol::table self = luaScript_;
    luaScript_[SCRIPT_CALLBACK_UPDATE](self);
}

void milk::Script::lateUpdate()
{
    sol::function onLateUpdate = luaScript_[SCRIPT_CALLBACK_LATE_UPDATE];

    if (!onLateUpdate.valid())
        return;

    sol::table self = luaScript_;
    luaScript_[SCRIPT_CALLBACK_LATE_UPDATE](self);
}

void milk::Script::onCollision(Collision& collision)
{
    sol::function onCollision = luaScript_[SCRIPT_CALLBACK_ON_COLLISION];

    if (!onCollision.valid())
        return;

    sol::table self = luaScript_;
    luaScript_[SCRIPT_CALLBACK_ON_COLLISION](self, collision);
}

void milk::Script::end()
{
    sol::function onEnd = luaScript_[SCRIPT_CALLBACK_END];

    if (!onEnd.valid())
        return;

    sol::table self = luaScript_;
    luaScript_[SCRIPT_CALLBACK_END](self);
}
