#ifndef MILK_LUAGAME_H
#define MILK_LUAGAME_H

#include <string>

namespace sol
{
    class state;
}

namespace milk
{
    class Game;
    class Scene;

    namespace lua
    {
        class LuaGame
        {
        public:
            static void bind(sol::state& luaState);

            static void loadScene(Game& game, const std::string& sceneToLoad);
        };
    }
}

#endif
