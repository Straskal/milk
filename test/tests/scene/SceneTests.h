#ifndef MILK_SCENE_TESTS_H
#define MILK_SCENE_TESTS_H

#include "gtest/gtest.h"

#include "mocks/asset/MockActorLoader.h"

using ::testing::_;

#include "scene/Scene.h"

class SceneTests : public ::testing::Test {};

TEST_F(SceneTests, Constructs)
{
    MockActorLoader actorLoader;

    ASSERT_NO_THROW(milk::Scene("scene", actorLoader));
}

TEST_F(SceneTests, SpawnActor_ReturnsSpawnedActor)
{
    MockActorLoader actorLoader;
    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{1, 1});

    ASSERT_NE(nullptr, actor);
    ASSERT_EQ("steve", actor->name());
    ASSERT_TRUE(milk::Vector2(1, 1) == actor->position());

    auto polledSpawn = scene.pollSpawned();

    ASSERT_EQ(actor, polledSpawn);
}

TEST_F(SceneTests, SpawnActorFromTemplate_ReturnsSpawnedActor)
{
    MockActorLoader actorLoader;

    EXPECT_CALL(actorLoader, load(_, "balls.json"))
        .Times(1);

    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{1, 1}, "balls.json");

    ASSERT_NE(nullptr, actor);
    ASSERT_EQ("steve", actor->name());
    ASSERT_TRUE(milk::Vector2(1, 1) == actor->position());

    auto polledSpawn = scene.pollSpawned();

    ASSERT_EQ(actor, polledSpawn);
}

TEST_F(SceneTests, DestroyActor_GivenSceneDoesNotContainActor_ReturnsFalse)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    bool destroyed = scene.destroyActor(1);

    ASSERT_FALSE(destroyed);
}

TEST_F(SceneTests, DestroyActor_GivenSceneContainsActor_ReturnsTrue)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{1, 1});

    bool destroyed = scene.destroyActor(actor->id());

    ASSERT_TRUE(destroyed);
}

TEST_F(SceneTests, DestroyActor_GivenSceneContainsActor_ActuallyDestroysActor)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{1, 1});

    bool destroyed = scene.destroyActor(actor->id());

    ASSERT_TRUE(destroyed);

    // Clear out spawned and internally add them to actorsById_ list.
    scene.pollSpawned();

    auto destroyedActor = scene.pollDestroyed();

    ASSERT_EQ(actor, destroyedActor);
}

#endif
