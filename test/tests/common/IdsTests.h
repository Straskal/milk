#ifndef MILK_IDSTESTS_H
#define MILK_IDSTESTS_H

#include "gtest/gtest.h"
#include "data/Ids.h"

namespace milk {
	class IdsTests : public ::testing::Test {};

	TEST_F(IdsTests, Creation) {
		Ids ids{};
		U32 id0 = ids.create();
		U32 id1 = ids.create();
		U32 id2 = ids.create();

		EXPECT_TRUE(ids.valid(id0));
		EXPECT_TRUE(ids.valid(id1));
		EXPECT_TRUE(ids.valid(id2));

		EXPECT_NE(Ids::INVALID, id0);
		EXPECT_NE(Ids::INVALID, id1);
		EXPECT_NE(Ids::INVALID, id2);
	}

	TEST_F(IdsTests, Removal) {
		Ids ids{};
		U32 id0 = ids.create();
		U32 id1 = ids.create();
		U32 id2 = ids.create();

		ids.remove(id0);
		ids.remove(id1);
		ids.remove(id2);

		EXPECT_FALSE(ids.valid(id0));
		EXPECT_FALSE(ids.valid(id1));
		EXPECT_FALSE(ids.valid(id2));
	}
}

#endif