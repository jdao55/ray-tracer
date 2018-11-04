#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"

using vec3 = geometry::vec<double, 3>;


vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0*vec3{drand48(),drand48(),0} - vec3{1,1,0};
    } while (p.dot(p) >= 1.0);
    return p;
}
class camera
{
  public:
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    double lens_radius;
    vec3 u, v, w;
    double time0, time1;

    camera(const vec3 & lookfrom, const vec3 & lookat, const vec3 & vup, const double vfov, const double aspect,
           double aperture, double focus_dist,const double t0, const double t1):time0(t0), time1(t1)
    {
        lens_radius = aperture / 2;
        double theta = vfov*M_PI/180;
        double half_height = tan(theta/2);
        double half_width = aspect * half_height;
        origin = lookfrom;
        w = (lookfrom - lookat).unit_vector();
        u = (cross(vup, w)).unit_vector();
        v = cross(w, u);
        lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
        horizontal = 2*half_width*focus_dist*u;
        vertical = 2*half_height*focus_dist*v;
    }
    geometry::Ray get_ray(double s, double t)
    {
        vec3 rd = lens_radius*random_in_unit_disk();
        vec3 offset = u * rd[0] + v * rd[1];
        double time = time0+drand48()*(time1-time0);
        return geometry::Ray{origin +offset, lower_left_corner + s*horizontal + t*vertical -origin -offset, time};
    }
};
#endif
