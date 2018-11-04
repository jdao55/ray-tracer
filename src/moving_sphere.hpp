
#ifndef MOVINGSPHERE_H
#define MOVINGSPHERE_H
#include "hitable.hpp"

class moving_sphere: public hitable
{
  public:
    vec3 center0, center1;
    double time0, time1, radius;
    std::shared_ptr<material> mat_ptr;


    moving_sphere(){};
    moving_sphere(vec3 cen0, vec3 cen1, double t0, double t1, double rad, std::shared_ptr<material> m):
            center0(cen0), center1(cen1),
            time0(t0), time1(t1),
            radius(rad),
            mat_ptr(m) {}

    bool hit (const geometry::Ray &r, double t_min, double t_max, hit_record &rec) const
    {
        vec3 oc = r.origin() -center(r.time);
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
                rec.normal = (rec.p -center(r.time)) /radius;
                rec.mat_ptr= mat_ptr;
                return true;

            }
            temp = (-b + sqrt(discriminant))/a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center(r.time)) /radius;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
        return false;
    }

    bool bounding_box(double t0, double t1, aabb& box) const {
        aabb box0(center(t0) - vec3{radius, radius, radius}, center(t0) + vec3{radius, radius, radius});
        aabb box1(center(t1) - vec3{radius, radius, radius}, center(t1) + vec3{radius, radius, radius});
        box = surrounding_box(box0, box1);
        return true;
    }


    vec3 center(double time) const
    {
        return center0 + ((time-time0) / (time1 - time0)) * (center1-center0);
    }
};
#endif
