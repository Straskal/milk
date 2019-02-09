#ifndef MILK_SCENEJSONPARSER_H
#define MILK_SCENEJSONPARSER_H

#include <memory>

#include "json/json.hpp"

namespace milk
{
    class Scene;

    namespace adapter
    {
        class SceneJsonDeserializer
        {
        public:
            virtual std::unique_ptr<Scene> deserialize(nlohmann::json& sceneJson) const = 0;
        };
    }
}

#endif
