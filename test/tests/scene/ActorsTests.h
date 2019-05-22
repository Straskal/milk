#ifndef MILK_ACTORS_TESTS
#define MILK_ACTORS_TESTS

#include "gtest/gtest.h"
#include "scene/Actors.h"

namespace milk {
	TEST(ActorsTests, Creation) {
		Actors actors{};
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		EXPECT_NE(Ids::INVALID, actor.id);
		EXPECT_NE(Ids::INVALID, actor1.id);
		EXPECT_NE(Ids::INVALID, actor2.id);

		EXPECT_NE(actor.id, actor1.id);
		EXPECT_NE(actor.id, actor2.id);
		EXPECT_NE(actor1.id, actor2.id);

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

	TEST(ActorsTests, Destruction) {
		Actors actors{};
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

	TEST(ActorsTests, Tags) {
		Actors actors{};
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		U32 playerTag = 1 << 0;
		U32 enemyTag = 1 << 1;
		U32 damagableTag = 1 << 2;

		actors.tag(actor, playerTag);
		actors.tag(actor1, enemyTag);
		actors.tag(actor2, damagableTag);

		EXPECT_TRUE(actors.tagged(actor, playerTag));
		EXPECT_TRUE(actors.tagged(actor1, enemyTag));
		EXPECT_TRUE(actors.tagged(actor2, damagableTag));

		actors.untag(actor, playerTag);
		actors.untag(actor1, enemyTag);
		actors.untag(actor2, damagableTag);

		EXPECT_FALSE(actors.tagged(actor, playerTag));
		EXPECT_FALSE(actors.tagged(actor1, enemyTag));
		EXPECT_FALSE(actors.tagged(actor2, damagableTag));
	}

	TEST(ActorsTests, GetByTag) {
		Actors actors{};
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		U32 playerTag = 1 << 0;
		U32 enemyTag = 1 << 1;
		U32 damagableTag = 1 << 2;

		actors.tag(actor, playerTag);
		actors.tag(actor1, enemyTag);
		actors.tag(actor2, enemyTag);

		actors.tag(actor, damagableTag);
		actors.tag(actor1, damagableTag);

		Array<Actor> players;
		actors.getByTag(players, playerTag);

		EXPECT_EQ(1, players.size());
		EXPECT_EQ(players[0].id, actor.id);

		Array<Actor> damagables;
		actors.getByTag(damagables, damagableTag);

		EXPECT_EQ(2, damagables.size());
		EXPECT_EQ(damagables[0].id, actor.id);
		EXPECT_EQ(damagables[1].id, actor1.id);

		Array<Actor> enemies;
		actors.getByTag(enemies, enemyTag);

		EXPECT_EQ(2, enemies.size());
		EXPECT_EQ(enemies[0].id, actor1.id);
		EXPECT_EQ(enemies[1].id, actor2.id);
	}

	TEST(ActorsTests, Position) {
		Actors actors{};
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor));
		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor1));
		EXPECT_EQ(Vector2::zero(), actors.getPosition(actor2));

		actors.setPosition(actor, Vector2{ 5.f, 6.f });
		actors.setPosition(actor1, Vector2{ 4.f, 9.f });
		actors.setPosition(actor2, Vector2{ 10.f, 1.f });

		EXPECT_EQ(Vector2(5.f, 6.f), actors.getPosition(actor));
		EXPECT_EQ(Vector2(4.f, 9.f), actors.getPosition(actor1));
		EXPECT_EQ(Vector2(10.f, 1.f), actors.getPosition(actor2));
	}

	TEST(ActorsTests, Name) {
		Actors actors{};
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		actors.setName(actor, "bobody");
		actors.setName(actor1, "burt macklin");
		actors.setName(actor2, "day man");

		EXPECT_EQ("bobody", actors.getName(actor));
		EXPECT_EQ("burt macklin", actors.getName(actor1));
		EXPECT_EQ("day man", actors.getName(actor2));
	}

	TEST(ActorsTests, GetByName) {
		Actors actors{};
		Actor actor = actors.create("stev");
		Actor actor1 = actors.create("tev");
		Actor actor2 = actors.create("ev");

		EXPECT_EQ(actor.id, actors.getByName("stev").id);
		EXPECT_EQ(actor1.id, actors.getByName("tev").id);
		EXPECT_EQ(actor2.id, actors.getByName("ev").id);
		EXPECT_EQ(Ids::INVALID, actors.getByName("jam").id);
	}
}

#endif