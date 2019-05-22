#ifndef MILK_RECTANGLETESTS_H
#define MILK_RECTANGLETESTS_H

#include "gtest/gtest.h"

#include "math/Rectangle.h"
TEST(Rectangle, Constructs) {
	ASSERT_NO_THROW(milk::Rectangle rect);
	ASSERT_NO_THROW(milk::Rectangle rect(0));
	ASSERT_NO_THROW(milk::Rectangle rect(0, 0));
	ASSERT_NO_THROW(milk::Rectangle rect(0, 0, 0));
	ASSERT_NO_THROW(milk::Rectangle rect(0, 0, 0, 0));
}

TEST(Rectangle, Top_ReturnsExpectedResult) {
	milk::Rectangle rect{ 0, 50, 0, 0 };

	int top = rect.top();

	ASSERT_EQ(50, top);
}

TEST(Rectangle, Bottom_ReturnsExpectedResult) {
	milk::Rectangle rect{ 0, 50, 0, 50 };

	int bottom = rect.bottom();

	ASSERT_EQ(100, bottom);
}

TEST(Rectangle, Left_ReturnsExpectedResult) {
	milk::Rectangle rect{ 50, 0, 0, 0 };

	int left = rect.left();

	ASSERT_EQ(50, left);
}

TEST(Rectangle, Right_ReturnsExpectedResult) {
	milk::Rectangle rect{ 50, 0, 50, 0 };

	int right = rect.right();

	ASSERT_EQ(100, right);
}

TEST(Rectangle, Overlapping_GivenOverlappingWithAnotherRectangle_ReturnsTrue) {
	milk::Rectangle rect1{ 0, 0, 100, 100 };
	milk::Rectangle rect2{ 50, 50, 100, 100 };

	bool overlapping = rect1.overlaps(rect2);

	ASSERT_TRUE(overlapping);
}

TEST(Rectangle, Overlapping_GivenNotOverlappingWithAnotherRectangle_ReturnsFalse) {
	milk::Rectangle rect1{ 0, 0, 100, 100 };
	milk::Rectangle rect2{ 150, 150, 100, 100 };

	bool overlapping = rect1.overlaps(rect2);

	ASSERT_FALSE(overlapping);
}

TEST(Rectangle, Empty_GivenRectEmpty_ReturnsTrue) {
	milk::Rectangle rect1{ 0, 0, 0, 0 };

	bool empty = rect1.empty();

	ASSERT_TRUE(empty);
}

TEST(Rectangle, Empty_GivenRectEmpty_ReturnsFalse) {
	milk::Rectangle rect1{ 10, 10, 10, 10 };

	bool empty = rect1.empty();

	ASSERT_FALSE(empty);
}

#endif
