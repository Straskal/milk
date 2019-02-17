#ifndef MILK_MOCK_TEXTURE_LOADER_H
#define MILK_MOCK_TEXTURE_LOADER_H

// Google mock
#include "gmock/gmock.h"

#include "graphics/Texture.h"
#include "asset/AssetCache.h"

class MockTextureCache : public milk::AssetCache<milk::Texture>
{
public:
    MOCK_METHOD1(load, std::shared_ptr<milk::Texture>(const std::string&));
    MOCK_METHOD0(freeUnreferencedAssets, void());
};

#endif
