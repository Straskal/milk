#ifndef MILK_SCENE_TESTS_H
#define MILK_SCENE_TESTS_H

#include "gtest/gtest.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

class SceneTests : public ::testing::Test
{
};

TEST_F(SceneTests, Ctor_Constructs)
{
    // Act & Assert
    ASSERT_NO_THROW(milk::Scene{});
}

TEST_F(SceneTests, SpawnActor_SpawnsActor)
{
    // Arrange
    milk::Scene scene{};

    // Act
    auto actor = scene.spawnActor("steve");

    // Assert
    ASSERT_NE(nullptr, actor);
}

TEST_F(SceneTests, SpawnActor_SetsActorName)
{
    // Arrange
    milk::Scene scene{};

    // Act
    auto actor = scene.spawnActor("steve");

    // Assert
    ASSERT_EQ("steve", actor->name());
}

TEST_F(SceneTests, SpawnActor_SetsActorId)
{
    // Arrange
    milk::Scene scene{};

    // Act
    auto waddap = scene.spawnActor("waddap");
    auto boi = scene.spawnActor("boi");
    auto ahaha = scene.spawnActor("ahaha");

    // Assert
    ASSERT_EQ(0, waddap->id());
    ASSERT_EQ(1, boi->id());
    ASSERT_EQ(2, ahaha->id());
}

TEST_F(SceneTests, FindActor_FindsActor)
{
    // Arrange
    milk::Scene scene{};

    auto actor = scene.spawnActor("steve");

    // Act
    auto foundActor = scene.findActor("steve");

    // Assert
    ASSERT_EQ(actor, foundActor);
}

TEST_F(SceneTests, FindActor_GivenActorDoesntExist_DoesNotFindActor)
{
    // Arrange
    milk::Scene scene{};

    // Act
    auto foundActor = scene.findActor("steve");

    // Assert
    ASSERT_EQ(nullptr, foundActor);
}

#endif
