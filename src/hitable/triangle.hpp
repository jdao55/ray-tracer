#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.hpp"

using vec3 = geometry::vec<float,3>;

class triangle: public hitable
{
  public:
    std::array<vec3, 3> points;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    triangle() {};
    triangle(vec3 a, vec3 b, vec3 c, std::shared_ptr<material> mat):
        points({a,b,c}),
        mat_ptr(mat){}
    bool hit (const geometry::Ray &r,const  float t_min,const float t_max, hit_record &rec) const
    {
        //TODO implement hit calculation
        return false;
    }
    bool bounding_box([[maybe_unused]] float t0, [[maybe_unused]] float t1, aabb& box) const
    {
        vec3 p1{std::min({points[0][0],points[1][0],points[2][0]}),
                std::min({points[0][1],points[1][1],points[2][1]}),
                std::min({points[0][2],points[1][2],points[2][2]})};
        vec3 p2{std::max({points[0][0],points[1][0],points[2][0]}),
                std::max({points[0][1],points[1][1],points[2][1]}),
                std::max({points[0][2],points[1][2],points[2][2]})};
        box = aabb(p1, p2);
        return true;
    }
};


#endif
