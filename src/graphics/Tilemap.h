#ifndef MILK_TILEMAP_H
#define MILK_TILEMAP_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "math/Rectangle.h"

namespace milk
{
    class Texture;

    struct TileType
    {
        TileType(int x, int y, int size, bool coll, const std::string& n)
                : sourceRect({x, y, size, size}),
                  collidable(coll),
                  name(n)
        {
        }

        Rectangle sourceRect;
        bool collidable;
        std::string name;

        // Additional information about the type will go here.
        // i.e. triggerable, opacity, animations.
    };

    struct TileInstance
    {
        TileInstance(TileType& tile, int x, int y)
                : type(tile),
                  x(x),
                  y(y)
        {
        }

        int x;
        int y;

        TileType& type;
    };


    struct TileLayer
    {
        ~TileLayer()
        {
            for (auto& it : tiles)
            {
                delete it;
                it = nullptr;
            }
        }

        std::vector<TileInstance*> tiles;

        void addTile(TileType& type, int x, int y)
        {
            tiles.emplace_back(new TileInstance(type, x, y));
        }
    };

    struct Tilemap
    {
        Tilemap()
                : textureName(""),
                  width(0),
                  height(0),
                  tileSize(0),
                  texture(nullptr)
        {
        }

        ~Tilemap()
        {
            for (auto& it : tileTypes)
            {
                delete it.second;
                it.second = nullptr;
            }

            for (auto& it : layers)
            {
                delete it;
                it = nullptr;
            }
        }

        std::string textureName;
        int width;
        int height;
        int tileSize;

        std::shared_ptr<Texture> texture;

        std::unordered_map<int, TileType*> tileTypes;
        std::vector<TileLayer*> layers;

        TileType& addTileType(int id, int x, int y, bool collidable, const std::string& name)
        {
            auto type = new TileType(x, y, tileSize, collidable, name);
            tileTypes.insert(std::pair<int, TileType*>(id, type));

            return *type;
        }

        TileLayer& addLayer()
        {
            auto layer = new TileLayer();
            layers.emplace_back(layer);

            return *layer;
        }
    };
}

#endif
