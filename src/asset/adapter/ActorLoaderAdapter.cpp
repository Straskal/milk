#include "ActorLoaderAdapter.h"

#include "asset/adapter/ActorTemplateCacheAdapter.h"
#include "asset/adapter/deserializers-v1/ActorJsonDeserializerV1.h"

#include "filesystem/Filesystem.h"

#include "game/Game.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

milk::adapter::ActorLoaderAdapter::~ActorLoaderAdapter() = default;

void milk::adapter::ActorLoaderAdapter::load(Actor& actor, const std::string& templateName) const
{
    using json = nlohmann::json;

    auto& game = Game::getInstance();

    json& actorJson = *(game.actorTemplateCache().load(templateName));

    int version = actorJson["version"].get<int>();

    std::unique_ptr<ActorJsonDeserializer> parser;

    // We version these bad boys so we can make changes to the schema and all existing assets will still work.
    switch (version)
    {
        case 1:
            parser = std::make_unique<ActorJsonDeserializerV1>(game);
            break;
        default:
            break;
    }

    return parser->deserialize(actor, actorJson);
}
