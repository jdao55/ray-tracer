#include "geometry_util.hpp"
#include "vec.hpp"

using vec3 = geometry::vec <float, 3>;

bool geometry::point_in_triangle(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &p){
    //TODO
    return true;
}
float geometry::triangle_area(const vec3 &a, const vec3 &b, const vec3 &c)
{
    auto ab = a-b;
    auto ac = a-c;

    return length(cross(ab,ac))/2;
}
auto geometry::ray_plane_intersection(const Ray &r, vec3 p_point, vec3 p_normal){

}
