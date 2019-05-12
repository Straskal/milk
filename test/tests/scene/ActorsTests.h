#ifndef MILK_ACTORS_TESTS
#define MILK_ACTORS_TESTS

#include "gtest/gtest.h"
#include "scene/Actors.h"

namespace milk
{
	class ActorsTests : public ::testing::Test
	{
	};

	TEST_F(ActorsTests, Creation)
	{
		Actors actors;
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		EXPECT_EQ(0, actor.id);
		EXPECT_EQ(1, actor1.id);
		EXPECT_EQ(2, actor2.id);

		EXPECT_TRUE(actors.alive(actor));
		EXPECT_TRUE(actors.alive(actor1));
		EXPECT_TRUE(actors.alive(actor2));

		EXPECT_EQ("stev", actors.getName(actor));
		EXPECT_EQ("tev", actors.getName(actor1));
		EXPECT_EQ("ev", actors.getName(actor2));

		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor));
		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor1));
		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor2));
	}

	TEST_F(ActorsTests, Destruction)
	{
		Actors actors;
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		EXPECT_TRUE(actors.alive(actor));
		EXPECT_TRUE(actors.alive(actor1));
		EXPECT_TRUE(actors.alive(actor2));

		actors.destroy(actor);
		actors.destroy(actor1);
		actors.destroy(actor2);

		EXPECT_FALSE(actors.alive(actor));
		EXPECT_FALSE(actors.alive(actor1));
		EXPECT_FALSE(actors.alive(actor2));
	}

	TEST_F(ActorsTests, Tags)
	{
		Actors actors;
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		U32 playerTag = 0 << 0;
		U32 enemyTag = 0 << 1;
		U32 damagableTag = 0 << 2;

		actors.tag(actor, playerTag);
		actors.tag(actor1, enemyTag);
		actors.tag(actor2, damagableTag);

		EXPECT_TRUE(actors.isTagged(actor, playerTag));
		EXPECT_TRUE(actors.isTagged(actor1, enemyTag));
		EXPECT_TRUE(actors.isTagged(actor2, damagableTag));

		actors.untag(actor, playerTag);
		actors.untag(actor1, enemyTag);
		actors.untag(actor2, damagableTag);

		EXPECT_FALSE(actors.isTagged(actor, playerTag));
		EXPECT_FALSE(actors.isTagged(actor1, enemyTag));
		EXPECT_FALSE(actors.isTagged(actor2, damagableTag));
	}

	TEST_F(ActorsTests, Position)
	{
		Actors actors;
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor));
		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor1));
		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor2));

		actors.setPosition(actor, Vector2{ 5.f, 6.f });
		actors.setPosition(actor1, Vector2{ 4.f, 9.f });
		actors.setPosition(actor2, Vector2{ 10.f, 1.f });

		EXPECT_EQ(Vector2(5.f, 6.f ), actors.getPosition(actor));
		EXPECT_EQ(Vector2(4.f, 9.f), actors.getPosition(actor1));
		EXPECT_EQ(Vector2(10.f, 1.f), actors.getPosition(actor2));
	}
}

#endif