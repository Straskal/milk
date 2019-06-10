#include "gtest/gtest.h"

#include "math/Matrix3Tests.h"
#include "math/RectangleTests.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}