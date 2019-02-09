#ifndef MILK_ASSET_LOADER_H
#define MILK_ASSET_LOADER_H

#include <memory>
#include <string>

namespace milk
{
    template<class TAsset>
    class AssetCache
    {
    public:
        virtual ~AssetCache() = default;

        virtual std::shared_ptr<TAsset> load(const std::string& assetName) = 0;

        virtual void freeUnreferencedAssets() = 0;
    };
}

#endif
