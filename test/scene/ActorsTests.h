#ifndef MILK_ACTORS_TESTS
#define MILK_ACTORS_TESTS

#include "gtest/gtest.h"
#include "scene/Actors.h"

namespace milk {
	TEST(ActorsTests, Creation) {
		Actors actors;

		U32 actor0 = actors.createActor("hail satan", Vector2{ 0.f, 12.f });
		U32 actor1 = actors.createActor("stev", Vector2{ 15.5f, 98.f });
		U32 actor2 = actors.createActor("danny devito", Vector2{ 0.f, -1.f });

		EXPECT_NE(Actors::INVALID, actor0);
		EXPECT_NE(Actors::INVALID, actor1);
		EXPECT_NE(Actors::INVALID, actor2);

		EXPECT_NE(actor0, actor1);
		EXPECT_NE(actor0, actor2);
		EXPECT_NE(actor1, actor2);

		EXPECT_EQ("hail satan", actors.getActorName(actor0));
		EXPECT_EQ("stev", actors.getActorName(actor1));
		EXPECT_EQ("danny devito", actors.getActorName(actor2));

		EXPECT_EQ(Vector2(0.f, 12.f), actors.getActorPosition(actor0));
		EXPECT_EQ(Vector2(15.5f, 98.f), actors.getActorPosition(actor1));
		EXPECT_EQ(Vector2(0.f, -1.f), actors.getActorPosition(actor2));
	}

	TEST(ActorsTests, Destruction) {
		Actors actors;

		U32 actor0 = actors.createActor("hail satan", Vector2{ 0.f, 12.f });
		U32 actor1 = actors.createActor("stev", Vector2{ 15.5f, 98.f });
		U32 actor2 = actors.createActor("danny devito", Vector2{ 0.f, -1.f });

		EXPECT_TRUE(actors.isActorAlive(actor0));
		EXPECT_TRUE(actors.isActorAlive(actor1));
		EXPECT_TRUE(actors.isActorAlive(actor2));

		actors.destroyActor(actor0);
		actors.destroyActor(actor1);
		actors.destroyActor(actor2);

		EXPECT_FALSE(actors.isActorAlive(actor0));
		EXPECT_FALSE(actors.isActorAlive(actor1));
		EXPECT_FALSE(actors.isActorAlive(actor2));
	}

	TEST(ActorsTests, Position) {
		Actors actors;

		U32 actor0 = actors.createActor("hail satan", Vector2{ 0.f, 12.f });
		U32 actor1 = actors.createActor("stev", Vector2{ 15.5f, 98.f });
		U32 actor2 = actors.createActor("danny devito", Vector2{ 0.f, -1.f });

		actors.setActorPosition(actor0, Vector2::zero());
		actors.setActorPosition(actor1, Vector2{ 1.f, 1.f });
		actors.setActorPosition(actor2, Vector2{ -10.f, 20.f });

		EXPECT_EQ(Vector2::zero(), actors.getActorPosition(actor0));
		EXPECT_EQ(Vector2(1.f, 1.f), actors.getActorPosition(actor1));
		EXPECT_EQ(Vector2(-10.f, 20.f), actors.getActorPosition(actor2));
	}

	TEST(ActorsTests, Name) {
		Actors actors;

		U32 actor0 = actors.createActor("hail satan", Vector2{ 0.f, 12.f });
		U32 actor1 = actors.createActor("stev", Vector2{ 15.5f, 98.f });
		U32 actor2 = actors.createActor("danny devito", Vector2{ 0.f, -1.f });

		actors.setActorName(actor0, "bobody");
		actors.setActorName(actor1, "burt macklin");
		actors.setActorName(actor2, "day man");

		EXPECT_EQ("bobody", actors.getActorName(actor0));
		EXPECT_EQ("burt macklin", actors.getActorName(actor1));
		EXPECT_EQ("day man", actors.getActorName(actor2));
	}

	TEST(ActorsTests, Tags) {
		Actors actors;

		U32 actor0 = actors.createActor("hail satan", Vector2{ 0.f, 12.f });
		U32 actor1 = actors.createActor("stev", Vector2{ 15.5f, 98.f });
		U32 actor2 = actors.createActor("danny devito", Vector2{ 0.f, -1.f });

		U32 playerTag = 1 << 0;
		U32 enemyTag = 1 << 1;
		U32 interactabletag = 1 << 2;

		actors.setActorTags(actor0, playerTag);
		actors.setActorTags(actor1, enemyTag | interactabletag);

		EXPECT_EQ(playerTag, actors.getActorTags(actor0));
		EXPECT_EQ(enemyTag | interactabletag, actors.getActorTags(actor1));
		EXPECT_EQ(0, actors.getActorTags(actor2));
	}
}

#endif