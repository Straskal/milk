#ifndef MILK_SCENE_LOADER_H
#define MILK_SCENE_LOADER_H

#include <memory>

#include "asset/SceneLoader.h"

namespace milk
{
    namespace adapter
    {
        class SceneLoaderAdapter : public SceneLoader
        {
        public:
            static SceneLoaderAdapter& getInstance()
            {
                static SceneLoaderAdapter instance;
                return instance;
            }

            ~SceneLoaderAdapter();

            std::unique_ptr<Scene> load(const std::string& file) const override;

        private:
            SceneLoaderAdapter() = default;
        };
    }
}

#endif
