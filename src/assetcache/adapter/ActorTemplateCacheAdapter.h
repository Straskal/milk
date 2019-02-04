#ifndef MILK_ACTORTEMPLATECACHE_H
#define MILK_ACTORTEMPLATECACHE_H

#include <string>
#include <unordered_map>

#include "assetcache/AssetCache.h"

#include "externals/json.hpp"

using json = nlohmann::json;

namespace milk
{
    class Filesystem;

    namespace adapter
    {
        class ActorTemplateCacheAdapter : public AssetCache<json>
        {
        public:
            explicit ActorTemplateCacheAdapter(const std::string& rootDir, Filesystem& filesystem);

            std::shared_ptr<json> load(const std::string& actorTemplateName) override;

            void freeUnreferencedAssets() override;

            void free();

        private:
            Filesystem& filesystem_;
            std::unordered_map<std::string, std::shared_ptr<json>> templateCache_;
        };
    }
}

#endif
