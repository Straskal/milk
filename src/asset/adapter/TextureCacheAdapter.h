#ifndef MILK_TEXTURE_LOADER_H
#define MILK_TEXTURE_LOADER_H

#include <string>
#include <unordered_map>

#include "asset/AssetCache.h"

struct SDL_Renderer;

namespace milk
{
    class Texture;

    namespace adapter
    {
        class TextureCacheAdapter : public AssetCache<Texture>
        {
        public:
            static TextureCacheAdapter& getInstance()
            {
                static TextureCacheAdapter instance;
                return instance;
            }

            bool init();

            std::shared_ptr<Texture> load(const std::string& textureName) override;

            void freeUnreferencedAssets() override;

            void free();

        private:
            TextureCacheAdapter() = default;

            std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache_;
        };
    }
}

#endif
