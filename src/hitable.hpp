#ifndef HITABLE_H
#define HITABLE_H

#include "ray.hpp"
#include "aabb.hpp"
#include <memory>

using vec3 = geometry::vec<double,3>;


class material;

struct hit_record
{
    double t;
    vec3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;

};

class hitable
{
 public:
    virtual bool hit(const geometry::Ray& r, double t_min, double t_max, hit_record &rec) const = 0;
    virtual bool bounding_box(double t0, double t1, aabb& box) const = 0;
};

inline aabb surrounding_box(const aabb box0,const aabb box1)
{
    vec3 small{ffmin(box0._min[0], box1._min[0]),
               ffmin(box0._min[1], box1._min[1]),
               ffmin(box0._min[2], box1._min[2])

    };
    vec3 large{ffmax(box0._min[0], box1._min[0]),
               ffmax(box0._min[1], box1._min[1]),
               ffmax(box0._min[2], box1._min[2])

    };
    return aabb{small, large};
}

#endif
