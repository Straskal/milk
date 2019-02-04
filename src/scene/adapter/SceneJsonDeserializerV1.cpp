#include "SceneJsonDeserializerV1.h"

#include "assetcache/AssetCache.h"

#include "filesystem/Filesystem.h"

#include "game/Game.h"

#include "graphics/Tilemap.h"

#include "physics/BoxCollider.h"

#include "scene/Actor.h"
#include "scene/Scene.h"
#include "scene/adapter/ActorLoaderAdapter.h"

milk::adapter::SceneJsonDeserializerV1::SceneJsonDeserializerV1(milk::Game& game)
        : game_(game)
{
}

std::unique_ptr<milk::Scene> milk::adapter::SceneJsonDeserializerV1::deserialize(nlohmann::json& sceneJson) const
{
    using json = nlohmann::json;

    auto sceneName = sceneJson["name"].get<std::string>();

    auto scene = std::make_unique<Scene>(std::make_unique<ActorLoaderAdapter>(game_), 1, sceneName);;

    json tilemapJson = sceneJson["tilemap"];

    if (!tilemapJson.is_null())
    {
        scene->tilemap(std::make_unique<Tilemap>());

        auto tilemap = scene->tilemap();

        tilemap->textureName = tilemapJson["texture"].get<std::string>();
        tilemap->texture = game_.textureCache().load(tilemap->textureName);

        tilemap->width = tilemapJson["width"].get<int>();
        tilemap->height = tilemapJson["height"].get<int>();
        tilemap->tileSize = tilemapJson["tileSize"].get<int>();

        // Lets load the tile set from another file.
        auto tilesetFileName = tilemapJson["tileSet"].get<std::string>();
        std::string tileSetFileContents = game_.filesystem().contents(tilesetFileName);

        auto tileSetJson = json::parse(tileSetFileContents);

        // TODO: use this field from json
        // auto name = tileSetJson["name"].get<std::string>();

        for (const auto& tileTypeJson : tileSetJson["tileTypes"])
        {
            int id = tileTypeJson["id"].get<int>();
            auto tileTypeName = tileTypeJson["name"].get<std::string>();
            int sourcePosX = tileTypeJson["sourcePosition"]["x"].get<int>();
            int sourcePosY = tileTypeJson["sourcePosition"]["y"].get<int>();
            bool collidable = tileTypeJson["flags"]["collidable"].get<bool>();

            tilemap->addTileType(id, sourcePosX, sourcePosY, collidable, tileTypeName);
        }

        auto layersJson = tilemapJson["layers"];

        for (const auto& layerJson : layersJson)
        {
            auto& tileLayer = tilemap->addLayer();

            int rowIndex = 0;

            for (const auto& row : layerJson)
            {
                int columnIndex = 0;

                for (const auto& column : row)
                {
                    int typeId = column.get<int>();

                    int xPosition = columnIndex * tilemap->tileSize;
                    int yPosition = rowIndex * tilemap->tileSize;

                    auto tileType = tilemap->tileTypes.at(typeId);

                    tileLayer.addTile(*tileType, xPosition, yPosition);

                    if (tileType->collidable)
                    {
                        auto tileActor = scene->spawnActor(tileType->name);
                        tileActor->position(xPosition, yPosition);
                        tileActor->addComponent<BoxCollider>(tilemap->tileSize, tilemap->tileSize);
                    }

                    columnIndex++;
                }

                rowIndex++;
            }
        }
    }

    auto actorsJson = sceneJson["actors"];

    for (const auto& actorJson : actorsJson)
    {
        auto actorName = actorJson["name"].get<std::string>();
        int xPosition = actorJson["position"]["x"].get<int>();
        int yPosition = actorJson["position"]["y"].get<int>();
        auto actorTemplate = actorJson["template"].get<std::string>();

        auto actor = scene->spawnActorFromTemplate(actorName, actorTemplate);

        actor->position(xPosition, yPosition);
    }

    return scene;
}
