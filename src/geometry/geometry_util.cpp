#include "geometry_util.hpp"
#include "vec.hpp"
#include <optional>
#include <vector>

using vec3 = geometry::vec<float, 3>;

bool geometry::point_in_triangle(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &p)
{
    // TODO
    return true;
}
float geometry::triangle_area(const vec3 &a, const vec3 &b, const vec3 &c)
{
    auto ab = a - b;
    auto ac = a - c;

    return length(cross(ab, ac)) / 2;
}
std::optional<vec3> geometry::ray_plane_intersection(const Ray &r, vec3 p_point, vec3 p_normal)
{
    auto denominator = geometry::dot(p_normal, r.B);
    if (denominator > 1e-6)
    {
        auto p0l0 = p_point - r.A;
        auto t = geometry::dot(p0l0, p_normal);
        return r.point_at_parameter(t);
    }
    return {};
}
