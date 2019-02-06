#include "SceneLoaderAdapter.h"

#include "scene/adapter/deserializers-v1/SceneJsonDeserializerV1.h"

#include "filesystem/Filesystem.h"

#include "game/Game.h"

#include "scene/Scene.h"

milk::adapter::SceneLoaderAdapter::SceneLoaderAdapter(Game& game)
        : game_(game)
{
}

milk::adapter::SceneLoaderAdapter::~SceneLoaderAdapter() = default;

std::unique_ptr<milk::Scene> milk::adapter::SceneLoaderAdapter::load(const std::string& file) const
{
    using json = nlohmann::json;

    std::string sceneJsonString = game_.filesystem().contents(file);

    json sceneJson = json::parse(sceneJsonString);

    int version = sceneJson["version"].get<int>();

    std::unique_ptr<SceneJsonDeserializer> parser;

    // We version these bad boys so we can make changes to the schema and all existing assets will still work.
    switch (version)
    {
        case 1:
            parser = std::make_unique<SceneJsonDeserializerV1>(game_);
            break;
        default:
            break;
    }

    return parser->deserialize(sceneJson);
}
