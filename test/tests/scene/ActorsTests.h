#ifndef MILK_ACTORS_TESTS
#define MILK_ACTORS_TESTS

#include "scene/Actors.h"

#include "gtest/gtest.h"

namespace milk
{
	class ActorsTests : public ::testing::Test
	{
	};

	TEST_F(ActorsTests, Create_CreateMany)
	{
		std::vector<std::string> names(Actors::MAX_ACTORS);
		std::vector<U32> ids(Actors::MAX_ACTORS);

		Actors actors;
		actors.Create(names, ids);
		
		for (int i = 0; i < Actors::MAX_ACTORS; ++i)
		{
			ASSERT_EQ(i, ids[i]);
		}
	}

	TEST_F(ActorsTests, Alive_WhenAlive_ReturnsTrue)
	{
		std::vector<std::string> names(Actors::MAX_ACTORS);
		std::vector<U32> ids(Actors::MAX_ACTORS);

		Actors actors;
		actors.Create(names, ids);

		ASSERT_TRUE(actors.Alive(ids));
	}

	TEST_F(ActorsTests, Alive_WhenDead_ReturnsFalse)
	{
		std::vector<std::string> names(Actors::MAX_ACTORS);
		std::vector<U32> ids(Actors::MAX_ACTORS);

		Actors actors;
		actors.Create(names, ids);
		actors.Destroy(ids);

		ASSERT_FALSE(actors.Alive(ids));
	}

	class ActorUtilsTests : public ::testing::Test
	{
	};

	TEST_F(ActorUtilsTests, Create)
	{
		Actors actors;
		U32 id0 = ActorUtils::Create(actors, "steve");
		U32 id1 = ActorUtils::Create(actors, "steve");
		U32 id2 = ActorUtils::Create(actors, "steve");

		ASSERT_EQ(0, id0);
		ASSERT_EQ(1, id1);
		ASSERT_EQ(2, id2);
	}

	TEST_F(ActorUtilsTests, Destroy)
	{
		Actors actors;
		U32 id0 = ActorUtils::Create(actors, "steve");
		ActorUtils::Destroy(actors, id0);

		ASSERT_FALSE(ActorUtils::Alive(actors, id0));
	}
}

#endif