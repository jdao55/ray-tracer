#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "../util.hpp"
#include <random>
using vec3 = geometry::vec<float, 3>;


vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0*vec3{random_float(), random_float(), random_float()} - vec3{1,1,1};
    } while (geometry::dot(p,p) >= 1.0);
    return p;
}
class camera
{
public:
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    float lens_radius;
    vec3 u, v, w;
    float time0, time1;


    camera(const vec3 & lookfrom, const vec3 & lookat, const vec3 & vup, const float vfov, const float aspect,
           float aperture, float focus_dist,const float t0, const float t1):time0(t0), time1(t1)
    {
        lens_radius = aperture / 2;
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = (lookfrom - lookat).unit_vector();
        u = (cross(vup, w)).unit_vector();
        v = cross(w, u);
        lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
        horizontal = 2*half_width*focus_dist*u;
        vertical = 2*half_height*focus_dist*v;
    }
    geometry::Ray get_ray(const float s, const float t) const
    {
        vec3 rd = lens_radius*random_in_unit_disk();
        vec3 offset = u * rd[0] + v * rd[1];
        float time = time0+drand48()*(time1-time0);
        return geometry::Ray{origin +offset, lower_left_corner + s*horizontal + t*vertical -origin -offset, time};
    }
};
#endif
