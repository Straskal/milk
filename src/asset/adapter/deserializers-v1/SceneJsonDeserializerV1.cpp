#include "SceneJsonDeserializerV1.h"

#include "asset/AssetCache.h"

#include "filesystem/Filesystem.h"

#include "game/Game.h"

#include "physics/BoxCollider.h"

#include "scene/Actor.h"
#include "scene/Scene.h"
#include "asset/adapter/ActorLoaderAdapter.h"

milk::adapter::SceneJsonDeserializerV1::SceneJsonDeserializerV1(milk::Game& game)
        : game_(game)
{
}

milk::adapter::SceneJsonDeserializerV1::~SceneJsonDeserializerV1() = default;

std::unique_ptr<milk::Scene> milk::adapter::SceneJsonDeserializerV1::deserialize(nlohmann::json& sceneJson) const
{
    using json = nlohmann::json;

    auto sceneName = sceneJson["name"].get<std::string>();

    auto scene = std::make_unique<Scene>(sceneName, ActorLoaderAdapter::instance());;

    auto actorsJson = sceneJson["actors"];

    for (const auto& actorJson : actorsJson)
    {
        auto actorName = actorJson["name"].get<std::string>();
        auto xPosition = actorJson["position"]["x"].get<float>();
        auto yPosition = actorJson["position"]["y"].get<float>();
        auto actorTemplate = actorJson["template"].get<std::string>();

        scene->spawnActor(actorName, {xPosition, yPosition}, actorTemplate);
    }

    return scene;
}
