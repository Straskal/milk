#ifndef MILK_SCENE_LOADER_H
#define MILK_SCENE_LOADER_H

#include <memory>

#include "scene/SceneLoader.h"

namespace milk
{
    class Game;

    namespace adapter
    {
        class SceneLoaderAdapter : public SceneLoader
        {
        public:
            explicit SceneLoaderAdapter(Game& game);

            ~SceneLoaderAdapter();

            std::unique_ptr<Scene> load(const std::string& file) const override;

        private:
            Game& game_;
        };
    }
}

#endif
