#include <iostream>
#include "../geometry/vec.hpp"
#include "gtest/gtest.h"


TEST(vec_test, operation_plus)
{
    geometry::vec<int> a{3,-21,1};
    geometry::vec<int> b{1,2,-12};
    auto c = a + b;
    EXPECT_EQ(c[0], 4);
    EXPECT_EQ(c[1], -19);
    EXPECT_EQ(c[2], -11);
}

TEST(vec_test, operation_minus)
{
    geometry::vec<int> a{3,-21,1};
    geometry::vec<int> b{1,2,-12};
    auto c = a - b;
    EXPECT_EQ(c[0], 2);
    EXPECT_EQ(c[1], -23);
    EXPECT_EQ(c[2], 13);
}

TEST(vec_test, operation_mult)
{
    geometry::vec<int> a{6, 4 ,0};
    geometry::vec<int> b{5, -3, 3};
    auto c = a * b;
    EXPECT_EQ(c[0], 30);
    EXPECT_EQ(c[1], -12);
    EXPECT_EQ(c[2], 0);
}


TEST(vec_test, operation_div)
{

    geometry::vec<int> a{6, 4 ,0};
    geometry::vec<int> b{5, -3, 3};
    auto c = a / b;
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], -1);
    EXPECT_EQ(c[2], 0);
}
