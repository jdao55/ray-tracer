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

TEST(vec_test, operation_dot)
{
    geometry::vec<int> a3{6, 4 ,0};
    geometry::vec<int> b3{5, -3, 3};

    EXPECT_EQ(geometry::dot(a3,b3), 18);

    geometry::vec<int, 1> a1{6};
    geometry::vec<int, 1> b1{5};
    EXPECT_EQ(geometry::dot(a1,b1), 30);

    geometry::vec<int, 5> a5{6, 1, -2 , 4 , 21};
    geometry::vec<int, 5> b5{5, -2, 0, 22, -123};
    EXPECT_EQ(geometry::dot(a5,b5), -2467);


}
