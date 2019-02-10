#ifndef MILK_MOCK_ACTOR_LOADER_H
#define MILK_MOCK_ACTOR_LOADER_H

#include "gmock/gmock.h"

#include "asset/ActorLoader.h"

#include "scene/Actor.h"

class MockActorLoader : public milk::ActorLoader
{
public:
    MOCK_CONST_METHOD2(load, void(milk::Actor&, const std::string&));
};

#endif
