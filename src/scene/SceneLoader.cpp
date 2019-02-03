#include "SceneLoader.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

#include "SDL.h"

#include "scene/Actor.h"
#include "game/Game.h"
#include "scene/Scene.h"

#include "filesystem/Filesystem.h"

#include "externals/json.hpp"

#include "graphics/Animator.h"
#include "graphics/Sprite.h"

#include "physics/BoxCollider.h"
#include "physics/Velocity.h"

#include "scripting/Script.h"

#include "window/Window.h"
#include "window/Renderer.h"

milk::SceneLoader::SceneLoader(Game& game)
        : game_(game)
{
}

milk::SceneLoader::~SceneLoader() = default;

std::unique_ptr<milk::Scene> milk::SceneLoader::load(const std::string& file) const
{
    using json = nlohmann::json;

    auto& textureCache = game_.textureCache();

    auto sceneJsonString = game_.filesystem().contents(file);
    json sceneJson = json::parse(sceneJsonString);

    auto scene = std::make_unique<Scene>();
    auto& tilemap = scene->tilemap();

    tilemap.sourceImageFile = sceneJson["source"].get<std::string>();
    tilemap.width = sceneJson["width"].get<int>();
    tilemap.height = sceneJson["height"].get<int>();
    tilemap.tileSize = sceneJson["tileSize"].get<int>();

    auto& tilesetJson = sceneJson["tileset"];

    for (auto& it : tilesetJson.items())
    {
        auto& tileTypeJson = it.value();

        int tileTypeId = tileTypeJson["id"].get<int>();
        std::string name = tileTypeJson["name"].get<std::string>();
        int x = tileTypeJson["x"].get<int>();
        int y = tileTypeJson["y"].get<int>();
        bool collidable = tileTypeJson["collidable"].get<bool>();

        tilemap.addTileType(tileTypeId, x, y, collidable, name);
    }

    int rows = tilemap.height / tilemap.tileSize;
    int columns = tilemap.width / tilemap.tileSize;

    auto& layersJson = sceneJson["layers"];

    for (auto& it : layersJson.items())
    {
        auto& tilemapLayer = tilemap.addLayer();

        auto& layerJson = it.value();

        for (int i = 0; i < layerJson.size(); ++i)
        {
            auto& row = layerJson[i];

            for (int j = 0; j < row.size(); ++j)
            {
                auto& column = row[j];
                int typeId = column.get<int>();

                if (typeId > 0)
                {
                    auto tile = tilemap.tileTypes[typeId];

                    int x = j * tilemap.tileSize;
                    int y = i * tilemap.tileSize;

                    tilemapLayer.addTile(*tile, x, y);

                    if (tile->collidable)
                    {
                        auto actor = scene->spawnActor(tile->name);
                        actor->position((float)x, (float)y);

                        auto collider = actor->addComponent<BoxCollider>();
                        collider->width(tilemap.tileSize);
                        collider->height(tilemap.tileSize);
                    }
                }
            }
        }
    }

    auto& actorListJson = sceneJson["actors"];

    for (auto& actorItr : actorListJson.items())
    {
        auto& actorJson = actorItr.value();

        std::string name = actorJson["name"].get<std::string>();
        int x = actorJson["x"].get<int>();
        int y = actorJson["y"].get<int>();

        auto actor = scene->spawnActor(name);
        actor->position(x, y);

        auto& componentListJson = actorJson["components"];

        for (auto& componentItr : componentListJson.items())
        {
            auto& componentJson = componentItr.value();

            std::string type = componentJson["type"];

            if (type == "sprite")
            {
                std::string textureName = componentJson["textureName"].get<std::string>();

                auto& sourceRectJson = componentJson["sourceRect"];
                int x = sourceRectJson["x"].get<int>();
                int y = sourceRectJson["y"].get<int>();
                int w = sourceRectJson["w"].get<int>();
                int h = sourceRectJson["h"].get<int>();

                auto sprite = actor->addComponent<Sprite>(textureName);
                sprite->sourceRect(x, y, w, h);
                sprite->center();
            } else if (type == "velocity")
            {
                float x = componentJson["x"].get<float>();
                float y = componentJson["y"].get<float>();

                auto velocity = actor->addComponent<Velocity>();
                velocity->value(x, y);
            } else if (type == "script")
            {
                std::string scriptName = componentJson["name"].get<std::string>();

                auto script = actor->addComponent<Script>();
                script->script(scriptName);
            } else if (type == "collider")
            {
                int w = componentJson["w"].get<int>();
                int h = componentJson["h"].get<int>();
                int xOffset = componentJson["xOffset"].get<int>();
                int yOffset = componentJson["yOffset"].get<int>();

                auto collider = actor->addComponent<BoxCollider>();
                collider->width(w);
                collider->height(h);
                collider->offset(xOffset, yOffset);
                collider->center();
            } else if (type == "animator")
            {
                int r = componentJson["rows"].get<int>();
                int c = componentJson["columns"].get<int>();

                auto animator = actor->addComponent<Animator>(r, c);

                for (auto& animItr : componentJson["animations"].items())
                {
                    auto& animationJson = animItr.value();

                    std::string n = animationJson["name"].get<std::string>();
                    auto frames = animationJson["frames"].items();
                    auto frameVector = std::vector<int>();

                    Animation anim{n};

                    for (auto& frameItr : frames)
                        anim.frames.emplace_back(frameItr.value().get<int>());

                    animator->addAnimation(anim);
                }
            }
        }
    }

    tilemap.texture = textureCache.load(tilemap.sourceImageFile);

    return scene;
}
