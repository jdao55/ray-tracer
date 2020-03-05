#include "geometry_util.hpp"
#include "vec.hpp"
#include <optional>
#include <vector>

using vec3 = geometry::vec<float, 3>;

bool geometry::point_in_triangle(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &p, const float error)
{
    float area_abc = geometry::triangle_area(a, b, c);
    float area_pbc = geometry::triangle_area(p, b, c);
    float area_apc = geometry::triangle_area(a, p, c);
    float area_abp = geometry::triangle_area(a, b, p);

    float area_difference = area_abc - area_abp - area_pbc - area_apc;
    float relative_difference = area_difference / area_abc;
    if (fabs(relative_difference) < error)
        return true;
    return false;
}
float geometry::triangle_area(const vec3 &a, const vec3 &b, const vec3 &c)
{
    auto ab = a - b;
    auto ac = a - c;

    return length(cross(ab, ac)) / 2;
}
std::optional<float> geometry::ray_plane_intersection(const geometry::Ray &r, vec3 p_point, vec3 p_normal)
{
    auto denominator = geometry::dot(p_normal, r.B);
    if (fabs(denominator) > 1e-6f)
    {
        auto p0l0 = p_point - r.A;
        auto t = geometry::dot(p0l0, p_normal)/denominator;
        return t;
    }
    return {};
}
