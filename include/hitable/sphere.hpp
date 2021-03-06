#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.hpp"

using vec3 = geometry::vec<float,3>;

class sphere: public hitable
{
  public:
    vec3 center;
    float radius;
    std::unique_ptr<material> mat_ptr;
    sphere() {};
    sphere(vec3 cen, float rad, std::unique_ptr<material> mat):center(cen), radius(rad), mat_ptr(std::move(mat)){}

    bool hit (const geometry::Ray &r,const  float t_min,const float t_max, hit_record &rec) const override
    {
        const vec3 oc = r.origin() -center;
        const float a = geometry::dot(r.direction(), r.direction());
        const float b = geometry::dot(oc,r.direction());
        const float c = geometry::dot(oc,oc) - radius*radius;
        const float discriminant =  b*b -a*c;
        if(discriminant>0)
        {
            float temp = (-b -sqrt(discriminant))/a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p -center) /radius;
                rec.mat_ptr= mat_ptr.get();
                return true;

            }
            temp = (-b + sqrt(discriminant))/a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p -center) /radius;
                rec.mat_ptr = mat_ptr.get();
                return true;
            }
        }
        return false;
    }
    bool bounding_box([[maybe_unused]] float t0, [[maybe_unused]] float t1, aabb& box) const override
    {
        box = aabb(center - vec3{radius, radius, radius}, center + vec3{radius, radius, radius});
        return true;
    }
};


#endif
