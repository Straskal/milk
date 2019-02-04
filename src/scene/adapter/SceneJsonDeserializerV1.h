#ifndef MILK_SCENELOADERV1_H
#define MILK_SCENELOADERV1_H

#include "SceneJsonDeserializer.h"

namespace milk
{
    class Game;

    namespace adapter
    {
        class SceneJsonDeserializerV1 : public SceneJsonDeserializer
        {
        public:
            explicit SceneJsonDeserializerV1(Game& game);

            std::unique_ptr<Scene> deserialize(nlohmann::json& sceneJson) const override;

        private:
            Game& game_;
        };
    }
}

#endif
