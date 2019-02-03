#include "gtest/gtest.h"

#include "tests/math/RectangleTests.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}