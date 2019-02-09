#ifndef MILK_LOGIC_H
#define MILK_LOGIC_H

#include <unordered_map>

#include "sol/sol.hpp"

namespace milk
{
    class Actor;
    class Collision;
    class Script;

    /// Logic handles all Game specific logic through Lua scripts.
    class Logic
    {
    public:
        /// \param luaState: A reference to the Game's Lua State.
        explicit Logic(sol::state& luaState);

        /// Called once every frame. Calls update() method in Script components.
        void update();

        void onActorSpawned(Actor& actor);

        void onActorDestroyed(Actor& actor);

        void onActorCollision(Collision& collisionEvent);

        /// Called once at the end of each frame, right before rendering. Calls lateUpdate() method in Script components.
        /// This method is best used for any Camera manipulation.
        void lateUpdate();

        void flush();

    private:
        sol::state& luaState_;

        std::unordered_map<int, Script*> scriptByActorId_;
    };
}

#endif
