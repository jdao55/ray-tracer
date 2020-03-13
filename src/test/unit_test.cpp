#include <iostream>
#include "../../include/ray_tracer.hpp"
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

TEST(geometry_test, area)
{
    using namespace geometry;
    vec<float, 3> a{0.0, 0.0, 0.0};
    vec<float, 3> b{1.0, 0.0, 0.0};
    vec<float, 3> c{0.0, 1.0, 0.0};
    EXPECT_EQ(triangle_area(a, b, c), 0.5);
}

TEST(gemetry_test, point_in_tri)
{
    using namespace geometry;
    vec<float, 3> a{0.0, 0.0, 0.0};
    vec<float, 3> b{1.0, 0.0, 0.0};
    vec<float, 3> c{0.0, 1.0, 0.0};
    EXPECT_EQ(point_in_triangle(a, b, c, vec3{0.5, 0.25, 0.0}, 1e-6f), true);
    EXPECT_EQ(point_in_triangle(a, b, c, vec3{2, 1, 0.0}, 1e-6f), false);

}

TEST(gemetry_test, ray_plane_intersection)
{
    using namespace geometry;
    vec<float, 3> p{0.0, 0.0, 0.0}; //point on plane
    vec<float, 3> n{0.0, 0.0, 1.0}; //plane normal
    Ray r( vec<float, 3>{0.0,0.0, 1.0},  vec<float, 3>{0.0,0.0,-1.0});
    EXPECT_EQ(ray_plane_intersection(r, p, n), 1.0f);
}
