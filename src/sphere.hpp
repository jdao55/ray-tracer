#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.hpp"

using vec3 = geometry::vec<double,3>;

class sphere: public hitable
{
  public:
    vec3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
    sphere() {};
    sphere(vec3 cen, double rad, std::shared_ptr<material> mat):center(cen), radius(rad), mat_ptr(mat){}
    bool hit (const geometry::Ray &r, double t_min, double t_max, hit_record &rec) const
    {
        vec3 oc = r.origin() -center;
        double a = r.direction().dot(r.direction());
        double b = oc.dot(r.direction());
        double c = oc.dot(oc) - radius*radius;
        double discriminant =  b*b -a*c;
        if(discriminant>0)
        {
            double temp = (-b -sqrt(discriminant))/a;
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
    bool bounding_box(double t0, double t1, aabb& box) const
    {
        box = aabb(center - vec3{radius, radius, radius}, center + vec3{radius, radius, radius});
        return true;
    }
};


#endif
