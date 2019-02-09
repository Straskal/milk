#ifndef MILK_SCRIPT_H
#define MILK_SCRIPT_H

#define SCRIPT_CALLBACK_BEGIN "begin"
#define SCRIPT_CALLBACK_UPDATE "update"
#define SCRIPT_CALLBACK_LATE_UPDATE "late_update"
#define SCRIPT_CALLBACK_ON_COLLISION "on_collision"
#define SCRIPT_CALLBACK_END "destroy"

#include <string>

#include "externals/sol.hpp"

#include "scene/ActorComponent.h"

namespace milk
{
    class Collision;

    class Script : public ActorComponent
    {
    public:
        static const ComponentType type;

        /// A Script allows an Actor to be controlled via Lua scripts.
        /// \param actor: The Actor that the Script is attached to
        /// \param scriptName: The name of the Lua script to attach to the Actor
        explicit Script(Actor& actor, const std::string& scriptName);

        /// Loads the Lua script.
        /// \param luaState: A reference to the Game's Lua State
        void load(sol::state& luaState);

        /// Called once the actor has been spawned into the scene.
        virtual void begin();

        /// Called once per frame.
        virtual void update();

        /// Called at the end of each frame, right before rendering.
        virtual void lateUpdate();

        /// Called when a Collision has been detected.
        /// \param collisionEvent: The Collision for the Script to handle
        virtual void onCollision(Collision& collision);

        /// Called once before the Actor is about to be destroyed.
        virtual void end();

    private:
        std::string scriptName_;
        sol::table luaScript_;
    };
}

#endif
