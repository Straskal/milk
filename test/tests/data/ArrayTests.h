#ifndef MILK_ARRAY_TESTS_H
#define MILK_ARRAY_TESTS_H

#include "gtest/gtest.h"
#include "data/Array.h"

namespace milk {
	class ArrayTests : public ::testing::Test {};

	TEST_F(ArrayTests, InitializesExpectedSize) {
		Array<int> array(10);
		ASSERT_EQ(10, array.size());
	}

	TEST_F(ArrayTests, IndexOperator) {
		Array<int> array(10);
		for (int i = 0; i < array.size(); ++i) {
			array[i] = i;
		}
		for (int i = 0; i < array.size(); ++i) {
			EXPECT_EQ(i, array[i]);
		}
	}
}

#endif