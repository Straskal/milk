#ifndef MILK_SCENE_TESTS_H
#define MILK_SCENE_TESTS_H

#include "gtest/gtest.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

#include "events/EventQueue.h"
#include "events/GameEvents.h"

class SceneTests : public ::testing::Test
{
};

TEST_F(SceneTests, Ctor_Constructs)
{
    // Arrange
    milk::EventQueue eventQueue;

    // Act & Assert
    ASSERT_NO_THROW(milk::Scene{eventQueue});
}

TEST_F(SceneTests, SpawnActor_SpawnsActor)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    // Act
    auto actor = scene.spawnActor("steve");

    // Assert
    ASSERT_NE(nullptr, actor);
}

TEST_F(SceneTests, SpawnActor_SetsActorName)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    // Act
    auto actor = scene.spawnActor("steve");

    // Assert
    ASSERT_EQ("steve", actor->name());
}

TEST_F(SceneTests, SpawnActor_SetsActorId)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    // Act
    auto waddap = scene.spawnActor("waddap");
    auto boi = scene.spawnActor("boi");
    auto ahaha = scene.spawnActor("ahaha");

    // Assert
    ASSERT_EQ(0, waddap->id());
    ASSERT_EQ(1, boi->id());
    ASSERT_EQ(2, ahaha->id());

    // Clean up
    while (auto e = eventQueue.poll())
    {}
}

TEST_F(SceneTests, SpawnActor_GeneratesActorSpawnedEvent)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    auto actor = scene.spawnActor("steve");

    // Act
    auto spawnedEvent = eventQueue.poll();

    // Assert
    ASSERT_NE(nullptr, spawnedEvent);
    ASSERT_TRUE(spawnedEvent->type() == milk::GameEventType::ACTOR_SPAWNED);

    auto& spawnedEventActor = (dynamic_cast<milk::ActorSpawnedEvent*>(spawnedEvent))->actor();

    ASSERT_EQ(actor, &spawnedEventActor);
}

TEST_F(SceneTests, DestroyActor_GivenActorDoesntExist_DoesNotGenerateActorDestroyedEvent)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    // Act
    scene.destroyActor(76);

    auto destroyedEvent = eventQueue.poll();

    // Assert
    ASSERT_EQ(nullptr, destroyedEvent);

    // Clean up
    while (auto e = eventQueue.poll())
    {}
}

TEST_F(SceneTests, DestroyActor_GeneratesActorDestroyedEvent)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    auto actor = scene.spawnActor("steve");

    // Lets get rid of that spawned event
    eventQueue.poll();

    scene.syncActorLists();

    // Act
    scene.destroyActor(actor->id());

    // Assert
    auto destroyedEvent = eventQueue.poll();

    ASSERT_NE(nullptr, destroyedEvent);
    ASSERT_TRUE(destroyedEvent->type() == milk::GameEventType::ACTOR_DETROYED);

    auto& destroyedEventActor = (dynamic_cast<milk::ActorDestroyedEvent*>(destroyedEvent))->actor();

    ASSERT_EQ(actor, &destroyedEventActor);

    // Clean up
    while (auto e = eventQueue.poll())
    {}
}

TEST_F(SceneTests, FindActor_FindsActor)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    auto actor = scene.spawnActor("steve");

    scene.syncActorLists();

    // Act
    auto foundActor = scene.findActor("steve");

    // Assert
    ASSERT_EQ(actor, foundActor);

    // Clean up
    while (auto e = eventQueue.poll())
    {}
}

TEST_F(SceneTests, FindActor_GivenActorDoesntExist_DoesNotFindActor)
{
    // Arrange
    milk::EventQueue eventQueue;

    milk::Scene scene{eventQueue};

    // Act
    auto foundActor = scene.findActor("steve");

    // Assert
    ASSERT_EQ(nullptr, foundActor);
}

#endif
