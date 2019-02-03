#ifndef MILK_RECTANGLETESTS_H
#define MILK_RECTANGLETESTS_H

#include "gtest/gtest.h"

#include "math/Rectangle.h"

class Rectangle : public ::testing::Test {};

TEST_F(Rectangle, Constructs)
{
    ASSERT_NO_THROW(milk::Rectangle rect);
    ASSERT_NO_THROW(milk::Rectangle rect(0));
    ASSERT_NO_THROW(milk::Rectangle rect(0, 0));
    ASSERT_NO_THROW(milk::Rectangle rect(0, 0, 0));
    ASSERT_NO_THROW(milk::Rectangle rect(0, 0, 0, 0));
}

TEST_F(Rectangle, Top_ReturnsExpectedResult)
{
    milk::Rectangle rect{0, 50, 0, 0};

    int top = rect.top();

    ASSERT_EQ(50, top);
}

TEST_F(Rectangle, Bottom_ReturnsExpectedResult)
{
    milk::Rectangle rect{0, 50, 0, 50};

    int bottom = rect.bottom();

    ASSERT_EQ(100, bottom);
}

TEST_F(Rectangle, Left_ReturnsExpectedResult)
{
    milk::Rectangle rect{50, 0, 0, 0};

    int left = rect.left();

    ASSERT_EQ(50, left);
}

TEST_F(Rectangle, Right_ReturnsExpectedResult)
{
    milk::Rectangle rect{50, 0, 50, 0};

    int right = rect.right();

    ASSERT_EQ(100, right);
}

TEST_F(Rectangle, Overlapping_GivenOverlappingWithAnotherRectangle_ReturnsTrue)
{
    milk::Rectangle rect1{0, 0, 100, 100};
    milk::Rectangle rect2{50, 50, 100, 100};

    bool overlapping = rect1.overlaps(rect2);

    ASSERT_TRUE(overlapping);
}

TEST_F(Rectangle, Overlapping_GivenNotOverlappingWithAnotherRectangle_ReturnsFalse)
{
    milk::Rectangle rect1{0, 0, 100, 100};
    milk::Rectangle rect2{150, 150, 100, 100};

    bool overlapping = rect1.overlaps(rect2);

    ASSERT_FALSE(overlapping);
}

TEST_F(Rectangle, Empty_GivenRectEmpty_ReturnsTrue)
{
    milk::Rectangle rect1{0, 0, 0, 0};

    bool empty = rect1.empty();

    ASSERT_TRUE(empty);
}

TEST_F(Rectangle, Empty_GivenRectEmpty_ReturnsFalse)
{
    milk::Rectangle rect1{10, 10, 10, 10};

    bool empty = rect1.empty();

    ASSERT_FALSE(empty);
}

#endif
