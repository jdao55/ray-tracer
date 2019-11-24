#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.hpp"

using vec3 = geometry::vec<float,3>;

class sphere: public hitable
{
  public:
    vec3 center;
    float radius;
    std::shared_ptr<material> mat_ptr;
    sphere() {};
    sphere(vec3 cen, float rad, std::shared_ptr<material> mat):center(cen), radius(rad), mat_ptr(mat){}
    bool hit (const geometry::Ray &r,const  float t_min,const float t_max, hit_record &rec) const
    {
        const vec3 oc = r.origin() -center;
        const float a = r.direction().dot(r.direction());
        const float b = oc.dot(r.direction());
        const float c = oc.dot(oc) - radius*radius;
        const float discriminant =  b*b -a*c;
        if(discriminant>0)
        {
            float temp = (-b -sqrt(discriminant))/a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p -center) /radius;
                rec.mat_ptr= mat_ptr;
                return true;

            }
            temp = (-b + sqrt(discriminant))/a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p -center) /radius;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
        return false;
    }
    bool bounding_box(float t0, float t1, aabb& box) const
    {
        box = aabb(center - vec3{radius, radius, radius}, center + vec3{radius, radius, radius});
        return true;
    }
};


#endif
