//#ifndef MILK_IDSTESTS_H
//#define MILK_IDSTESTS_H
//
//#include "gtest/gtest.h"
//#include "data/Ids.h"
//
//namespace milk {
//	TEST(IdsTests, Creation) {
//		Ids actors{};
//		U32 id0 = ids.create();
//		U32 id1 = ids.create();
//		U32 id2 = ids.create();
//
//		EXPECT_TRUE(ids.valid(id0));
//		EXPECT_TRUE(ids.valid(id1));
//		EXPECT_TRUE(ids.valid(id2));
//
//		EXPECT_NE(Ids::INVALID, id0);
//		EXPECT_NE(Ids::INVALID, id1);
//		EXPECT_NE(Ids::INVALID, id2);
//	}
//
//	TEST(IdsTests, Removal) {
//		Ids ids{};
//		U32 id0 = ids.create();
//		U32 id1 = ids.create();
//		U32 id2 = ids.create();
//
//		ids.remove(id0);
//		ids.remove(id1);
//		ids.remove(id2);
//
//		EXPECT_FALSE(ids.valid(id0));
//		EXPECT_FALSE(ids.valid(id1));
//		EXPECT_FALSE(ids.valid(id2));
//	}
//}
//
//#endif