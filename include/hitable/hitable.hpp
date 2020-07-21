#ifndef HITABLE_H
#define HITABLE_H

#include "../geometry/ray.hpp"
#include "aabb.hpp"
#include <memory>

using vec3 = geometry::vec<float, 3>;


class material;

struct hit_record
{
    float t;
    float u;
    float v;
    vec3 p;
    vec3 normal;
    material* mat_ptr;

};

class hitable
{
 public:
    virtual bool hit(const geometry::Ray& r, float t_min, float t_max, hit_record &rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
    hitable() = default;
    virtual ~hitable() = default;
    //hitable &operator=(const hitable &) = delete;
    //hitable(const hitable &) = delete;
};

#endif
