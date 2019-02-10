#ifndef MILK_MATRIX3_TESTS_H
#define MILK_MATRIX3_TESTS_H

#include "gtest/gtest.h"

#include "math/Matrix3.h"

class Matrix3Tests : public ::testing::Test
{
};

TEST_F(Matrix3Tests, Constructs)
{
    ASSERT_NO_THROW(milk::Matrix3 matrix3);
    ASSERT_NO_THROW(milk::Matrix3 matrix3(1.0f, 1.0f, 1.0f,
                                          1.0f, 1.0f, 1.0f,
                                          1.0f, 1.0f, 1.0f));
}

TEST_F(Matrix3Tests, Identity)
{
    auto expectedResult = milk::Matrix3{1.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f};

    auto identity = milk::Matrix3::identity();

    ASSERT_EQ(expectedResult, identity);
}

TEST_F(Matrix3Tests, Multiplication)
{
    auto expectedResult = milk::Matrix3{20.0f, 8.0f, 12.0f,
                                        25.0f, 4.0f, 34.0f,
                                        2.0f,  0.0f, 4.0f};

    auto matrix1 = milk::Matrix3{4.0f, 2.0f, 0.0f,
                                 0.0f, 8.0f, 1.0f,
                                 0.0f, 1.0f, 0.0f};

    auto matrix2 = milk::Matrix3{4.0f, 2.0f, 1.0f,
                                 2.0f, 0.0f, 4.0f,
                                 9.0f, 4.0f, 2.0f};

    auto result = matrix1 * matrix2;

    ASSERT_EQ(expectedResult, result);
}

#endif
