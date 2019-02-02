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
    class ActorCollisionEvent;

    class Script : public ActorComponent
    {
    public:
        static const ComponentType type;

        /// A Script is what controls an actor's behavior.
        /// \param actor
        explicit Script(Actor& actor);

        ~Script() override = default;

        /// Sets the name of the lua script to load
        /// \param scriptName: The name of the script to load
        void script(const std::string& scriptName);

        /// Loads the lua script
        /// \param luaState: A reference to the Game's Lua State
        void load(sol::state& luaState);

        /// Called once the actor has been spawned into the scene.
        virtual void begin();

        /// Called once per frame.
        virtual void update();

        /// Called at the end of frame, right before rendering.
        virtual void lateUpdate();

        /// Called when a collision has been detected.
        /// \param collisionEvent: The CollisionEvent for the Script to handle
        virtual void onCollision(ActorCollisionEvent& collisionEvent);

        /// Called once before the actor is about to be destroyed.
        virtual void end();

    private:
        std::string scriptName_;
        sol::table luaScript_;
    };
}

#endif
