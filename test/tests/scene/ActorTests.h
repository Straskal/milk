#ifndef MILK_ACTOR_TESTS_H
#define MILK_ACTOR_TESTS_H

#include "gtest/gtest.h"

#include "mocks/asset/MockActorLoader.h"
#include "mocks/scene/MockActorComponent.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

class ActorTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockActorLoader_ = new MockActorLoader();
        scene_ = new milk::Scene("scene", *mockActorLoader_);
    }

    void TearDown() override
    {
        delete scene_;
        delete mockActorLoader_;
    }

    MockActorLoader* mockActorLoader_;
    milk::Scene* scene_;
};

TEST_F(ActorTests, Ctor_Constructs)
{
    ASSERT_NO_THROW(milk::Actor actor(*scene_, 1, "steve", milk::Vector2{}));
}

TEST_F(ActorTests, Scene_ReturnsScene)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    auto& scene = actor.scene();

    ASSERT_EQ(scene_, &scene);
}


TEST_F(ActorTests, Id_ReturnsId)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    int id = actor.id();

    ASSERT_EQ(1, id);
}

TEST_F(ActorTests, Name_ReturnsName)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    std::string name = actor.name();

    ASSERT_EQ("steve", name);
}

TEST_F(ActorTests, Position_ReturnsPosition)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{50, 50});

    milk::Vector2 pos = actor.position();

    ASSERT_EQ(milk::Vector2(50, 50), pos);
}

TEST_F(ActorTests, Position_SetsPosition)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    actor.position(50, 50);

    milk::Vector2 pos = actor.position();

    ASSERT_EQ(milk::Vector2(50, 50), pos);
}

TEST_F(ActorTests, AddComponent_GivenActorDoesNotContainComponent_ComponentIsAdded)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    auto newlyAdded = actor.addComponent<MockActorComponent>();

    ASSERT_NE(nullptr, newlyAdded);
}

TEST_F(ActorTests, AddComponent_GivenActorContainsComponent_ComponentIsNotAdded)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    actor.addComponent<MockActorComponent>();

    auto newlyAdded = actor.addComponent<MockActorComponent>();

    ASSERT_EQ(nullptr, newlyAdded);
}

TEST_F(ActorTests, GetComponent_GivenActorContainsComponent_ComponentIsReturned)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    actor.addComponent<MockActorComponent>();

    auto component = actor.getComponent<MockActorComponent>();

    ASSERT_NE(nullptr, component);
}

TEST_F(ActorTests, GetComponent_GivenActorDoesNotContainComponent_ReturnsNothing)
{
    milk::Actor actor(*scene_, 1, "steve", milk::Vector2{});

    auto component = actor.getComponent<MockActorComponent>();

    ASSERT_EQ(nullptr, component);
}

#endif
