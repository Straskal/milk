#ifndef MILK_LUAGAME_H
#define MILK_LUAGAME_H

#include <string>

namespace milk
{
    class Game;
    class Scene;

    namespace lua
    {
        namespace game
        {
            void loadScene(Game& game, const std::string& sceneToLoad);
        }
    }
}

#endif
