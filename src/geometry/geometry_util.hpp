#ifndef GUTIL_H
#define GUTIL_H

#include "ray.hpp"

namespace geometry {
    using vec3 = vec<float, 3>;
    bool point_in_triangle(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &p);
    auto triangle_area(const vec3 &a, const vec3 &b, const vec3 &c);
    auto ray_plane_intersection(const Ray &r, vec3 p_point, vec3 p_normal);
};
#endif
