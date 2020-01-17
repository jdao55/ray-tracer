#include <iostream>
#include "../geometry/vec.hpp"
#include "gtest/gtest.h"


TEST(vec_test, operations)
{
    geometry::vec<int> a{1,1,1};
    geometry::vec<int> b{1,2,3};
    auto c = a + b;
    EXPECT_EQ(c[0], 2);
    EXPECT_EQ(c[1], 3);
    EXPECT_EQ(c[2], 4);

}
