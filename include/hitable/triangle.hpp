#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitable.hpp"
#include "../geometry/geometry_util.hpp"

using vec3 = geometry::vec<float, 3>;

class triangle : public hitable
{
  public:
    std::array<vec3, 3> points;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    triangle(vec3 a, vec3 b, vec3 c, std::shared_ptr<material> mat)
        : points({ a, b, c }), normal(find_normal()), mat_ptr(mat)
    {}
    bool hit(const geometry::Ray &r, const float t_min, const float t_max, hit_record &rec) const override
    {
        auto t = geometry::ray_plane_intersection(r, points[0], normal);
        if (!t.has_value()) return false;
        if (t.value() < t_min || t.value() > t_max) return false;
        vec3 p = r.point_at_parameter(t.value());
        if (geometry::point_in_triangle(points[0], points[1], points[2], p, 1e-6f))
        {
            rec.p = p;
            rec.t = t.value();
            rec.normal = normal;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        return false;
    }
    bool bounding_box([[maybe_unused]] float t0, [[maybe_unused]] float t1, aabb &box) const override
    {
        vec3 p1{ std::min({ points[0][0], points[1][0], points[2][0] }),
            std::min({ points[0][1], points[1][1], points[2][1] }),
            std::min({ points[0][2], points[1][2], points[2][2] }) };
        vec3 p2{ std::max({ points[0][0], points[1][0], points[2][0] }),
            std::max({ points[0][1], points[1][1], points[2][1] }),
            std::max({ points[0][2], points[1][2], points[2][2] }) };
        box = aabb(p1, p2);
        return true;
    }
    vec3 find_normal() { return geometry::cross(points[1] - points[0], points[2] - points[0]).unit_vector(); }
};


#endif
