#include "gtest/gtest.h"

#include "data/IdsTests.h"
#include "math/Matrix3Tests.h"
#include "math/RectangleTests.h"
#include "scene/ActorsTests.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}