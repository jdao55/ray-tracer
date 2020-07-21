#ifndef MOVINGSPHERE_H
#define MOVINGSPHERE_H
#include "hitable.hpp"

class moving_sphere: public hitable
{
  public:
    vec3 center0, center1;
    float time0, time1, radius;
    std::unique_ptr<material> mat_ptr;


    moving_sphere(){};
    moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float rad, std::unique_ptr<material> m):
            center0(cen0), center1(cen1),
            time0(t0), time1(t1),
            radius(rad),
            mat_ptr(std::move(m)) {}

    bool hit (const geometry::Ray &r, float t_min, float t_max, hit_record &rec) const override
    {
        const vec3 oc = r.origin() -center(r.time);
        const float a = geometry::dot(r.direction(), r.direction());
        const float b = geometry::dot(oc, r.direction());
        const float c = geometry::dot(oc, oc) - radius * radius;
        const float discriminant =  b*b -a*c;
        if(discriminant>0)
        {
            float temp = (-b -sqrt(discriminant))/a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p -center(r.time)) /radius;
                rec.mat_ptr= mat_ptr.get();
                return true;

            }
            temp = (-b + sqrt(discriminant))/a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center(r.time)) /radius;
                rec.mat_ptr = mat_ptr.get();
                return true;
            }
        }
        return false;
    }

    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        aabb box0(center(t0) - vec3{radius, radius, radius}, center(t0) + vec3{radius, radius, radius});
        aabb box1(center(t1) - vec3{radius, radius, radius}, center(t1) + vec3{radius, radius, radius});
        box = surrounding_box(box0, box1);
        return true;
    }


    vec3 center(float time) const
    {
        return center0 + ((time-time0) / (time1 - time0)) * (center1-center0);
    }
};
#endif
