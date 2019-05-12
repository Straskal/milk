#ifndef MILK_FASTMAPTESTS_H
#define MILK_FASTMAPTESTS_H

#include "gtest/gtest.h"
#include "common/FastMap.h"
#include "common/String.h"

namespace milk 
{
	class FastMapTests : public ::testing::Test
	{
	};

	TEST_F(FastMapTests, Push)
	{
		FastMap<String> fm{};
		fm.push(10, "ten");
		fm.push(6, "six");
		fm.push(27, "twentyseven");

		EXPECT_EQ(3, fm.size());
	}

	TEST_F(FastMapTests, Lookup) 
	{
		FastMap<String> fm{};
		fm.push(10, "ten");
		fm.push(6, "six");
		fm.push(27, "twentyseven");

		EXPECT_EQ("ten", fm.lookup(10));
		EXPECT_EQ("six", fm.lookup(6));
		EXPECT_EQ("twentyseven", fm.lookup(27));
	}

	TEST_F(FastMapTests, Remove)
	{
		FastMap<String> fm{};
		fm.push(10, "ten");
		fm.push(6, "six");
		fm.push(27, "twentyseven");

		fm.remove(10);
		fm.remove(6);
		fm.remove(27);

		EXPECT_EQ(0, fm.size());
	}

	TEST_F(FastMapTests, Index)
	{
		FastMap<String> fm{};
		fm.push(0, "ten");
		fm.push(3, "six");
		fm.push(6, "twentyseven");

		EXPECT_EQ("ten", fm[0]);
		EXPECT_EQ("six", fm[1]);
		EXPECT_EQ("twentyseven", fm[2]);
	}
}

#endif