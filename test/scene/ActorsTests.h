#ifndef MILK_ACTORS_TESTS
#define MILK_ACTORS_TESTS

#include "gtest/gtest.h"
#include "scene/Actors.h"

namespace milk {
	TEST(ActorsTests, Creation) {
		ActorData actorData;

		U32 actor0 = actor::create(actorData, "hail satan");
		U32 actor1 = actor::create(actorData, "stev");
		U32 actor2 = actor::create(actorData, "dumbo");

		EXPECT_NE(id::INVALID, actor0);
		EXPECT_NE(id::INVALID, actor1);
		EXPECT_NE(id::INVALID, actor2);

		EXPECT_NE(actor0, actor1);
		EXPECT_NE(actor0, actor2);
		EXPECT_NE(actor1, actor2);

		EXPECT_EQ("hail satan", actor::getName(actorData, actor0));
		EXPECT_EQ("stev", actor::getName(actorData, actor1));
		EXPECT_EQ("dumbo", actor::getName(actorData, actor2));

		EXPECT_TRUE(id::valid(actorData.ids, actor0));
		EXPECT_TRUE(id::valid(actorData.ids, actor1));
		EXPECT_TRUE(id::valid(actorData.ids, actor2));
	}

	TEST(ActorsTests, Destruction) {
		ActorData actorData;

		U32 actor0 = actor::create(actorData, "hail satan");
		U32 actor1 = actor::create(actorData, "stev");
		U32 actor2 = actor::create(actorData, "dumbo");

		EXPECT_TRUE(id::valid(actorData.ids, actor0));
		EXPECT_TRUE(id::valid(actorData.ids, actor1));
		EXPECT_TRUE(id::valid(actorData.ids, actor2));

		actor::destroy(actorData, actor0);
		actor::destroy(actorData, actor1);
		actor::destroy(actorData, actor2);

		EXPECT_FALSE(id::valid(actorData.ids, actor0));
		EXPECT_FALSE(id::valid(actorData.ids, actor1));
		EXPECT_FALSE(id::valid(actorData.ids, actor2));

		EXPECT_EQ(3, actorData.destroyed.size());
	}

	TEST(ActorsTests, Name) {
		ActorData actorData;

		U32 actor0 = actor::create(actorData, "hail satan");
		U32 actor1 = actor::create(actorData, "stev");
		U32 actor2 = actor::create(actorData, "dumbo");

		actor::setName(actorData, actor0, "bobody");
		actor::setName(actorData, actor1, "burt macklin");
		actor::setName(actorData, actor2, "day man");

		EXPECT_EQ("bobody", actor::getName(actorData, actor0));
		EXPECT_EQ("burt macklin", actor::getName(actorData, actor1));
		EXPECT_EQ("day man", actor::getName(actorData, actor2));
	}

	TEST(ActorsTests, GetByName) {
		ActorData actorData;

		U32 actor0 = actor::create(actorData, "hail satan");
		U32 actor1 = actor::create(actorData, "stev");
		U32 actor2 = actor::create(actorData, "dumbo");

		EXPECT_EQ(actor0, actor::getByName(actorData, "hail satan"));
		EXPECT_EQ(actor1, actor::getByName(actorData, "stev"));
		EXPECT_EQ(actor2, actor::getByName(actorData, "dumbo"));
		EXPECT_EQ(id::INVALID, actor::getByName(actorData, "jam"));
	}
}

#endif