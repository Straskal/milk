#ifndef MILK_LOGIC_H
#define MILK_LOGIC_H

#include <unordered_map>

#include "externals/sol.hpp"

namespace milk
{
    class Actor;
    class Collision;
    class Script;

    class Logic
    {
    public:
        /// Logic handles all Game specific logic through Lua Scripts.
        /// \param luaState: A reference to the Game's Lua State.
        explicit Logic(sol::state& luaState);

        ~Logic() = default;

        /// Called once every frame. Calls update() method in Script components.
        void update();

        void onActorSpawned(Actor& actor);

        void onActorDestroyed(Actor& actor);

        void onActorCollision(Collision& collisionEvent);

        /// Called once at the end of each frame. Calls lateUpdate() method in Script components.
        /// This method is best used for any Camera manipulation.
        void lateUpdate();

        void flush();

    private:
        sol::state& luaState_;

        std::unordered_map<int, Script*> scriptByActorId_;
    };
}

#endif
