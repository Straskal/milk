#ifndef MILK_ACTORTEMPLATECACHE_H
#define MILK_ACTORTEMPLATECACHE_H

#include <string>
#include <unordered_map>

#include "asset/AssetCache.h"

#include "json/json.hpp"

using json = nlohmann::json;

namespace milk
{
    class Filesystem;

    namespace adapter
    {
        class ActorTemplateCacheAdapter : public AssetCache<json>
        {
        public:
            static ActorTemplateCacheAdapter& getInstance()
            {
                static ActorTemplateCacheAdapter instance;
                return instance;
            }

            std::shared_ptr<json> load(const std::string& actorTemplateName) override;

            void freeUnreferencedAssets() override;

            void free();

        private:
            ActorTemplateCacheAdapter() = default;

            std::unordered_map<std::string, std::shared_ptr<json>> templateCache_;
        };
    }
}

#endif
