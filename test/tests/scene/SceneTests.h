#ifndef MILK_SCENE_TESTS_H
#define MILK_SCENE_TESTS_H

#include "gtest/gtest.h"

#include "mocks/asset/MockActorLoader.h"

using ::testing::_;

#include "scene/Scene.h"

TEST(SceneTests, Constructs)
{
    MockActorLoader actorLoader;

    ASSERT_NO_THROW(milk::Scene("scene", actorLoader));
}

TEST(SceneTests, SpawnActor_ReturnsSpawnedActor)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{});

    ASSERT_NE(nullptr, actor);
}

TEST(SceneTests, SpawnActor_AssignsProperValuesToNewlySpawnedActor)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{1, 1});

    ASSERT_EQ("steve", actor->name());
    ASSERT_TRUE(milk::Vector2(1, 1) == actor->position());
}

TEST(SceneTests, SpawnActorFromTemplate_ReturnsSpawnedActor)
{
    MockActorLoader actorLoader;

    EXPECT_CALL(actorLoader, load(_, "hi.json"))
        .Times(1);

    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{1, 1}, "hi.json");

    ASSERT_NE(nullptr, actor);
}

TEST(SceneTests, PollSpawned_ReturnsSpawnedActors)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    auto actor1 = scene.spawnActor("steve1", milk::Vector2{1, 1});
    auto actor2 = scene.spawnActor("steve2", milk::Vector2{1, 1});

    auto polledSpawn1 = scene.pollSpawned();
    auto polledSpawn2 = scene.pollSpawned();
    auto polledSpawn3 = scene.pollSpawned();

    ASSERT_EQ(actor2, polledSpawn1);
    ASSERT_EQ(actor1, polledSpawn2);
    ASSERT_EQ(nullptr, polledSpawn3);
}

TEST(SceneTests, DestroyActor_GivenSceneDoesNotContainActor_ReturnsFalse)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    bool destroyed = scene.destroyActor(1);

    ASSERT_FALSE(destroyed);
}

TEST(SceneTests, DestroyActor_GivenSceneContainsActor_ReturnsTrue)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    auto actor = scene.spawnActor("steve", milk::Vector2{1, 1});

    bool destroyed = scene.destroyActor(actor->id());

    ASSERT_TRUE(destroyed);
}

TEST(SceneTests, PollDestroyed_ReturnsDestroyedActors)
{
    MockActorLoader actorLoader;

    milk::Scene scene{"scene", actorLoader};

    auto actor1 = scene.spawnActor("steve", milk::Vector2{1, 1});
    auto actor2 = scene.spawnActor("steve", milk::Vector2{1, 1});

    scene.destroyActor(actor1->id());
    scene.destroyActor(actor2->id());

    // Clear out spawned and internally add them to actorsById_ list.
    scene.pollSpawned();
    scene.pollSpawned();

    auto destroyedActor1 = scene.pollDestroyed();
    auto destroyedActor2 = scene.pollDestroyed();

    ASSERT_EQ(actor2, destroyedActor1);
    ASSERT_EQ(actor1, destroyedActor2);
}

#endif
