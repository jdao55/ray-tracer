#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "../util.hpp"

using vec3 = geometry::vec<float, 3>;


vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0f*vec3{random_float(), random_float(), random_float()} - vec3{1.0f,1.0f,1.0f};
    } while (geometry::dot(p,p) >= 1.0f);
    return p;
}
class camera
{
private:
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    float lens_radius;
    vec3 u, v, w;
    float time0, time1;

public:
    camera(const vec3 & lookfrom, const vec3 & lookat, const vec3 & vup, const float vfov, const float aspect,
           float aperture, float focus_dist,const float t0, const float t1):time0(t0), time1(t1)
    {

        const float theta = vfov* static_cast<float>(M_PI) /180.0F;
        const float half_height = tan(theta/2.0f);
        const float half_width = aspect * half_height;

        w = (lookfrom - lookat).unit_vector();
        u = (cross(vup, w)).unit_vector();
        v = cross(w, u);
        lens_radius = aperture / 2.0F;

        origin = lookfrom;
        lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
        horizontal = 2.0F * half_width*focus_dist*u;
        vertical = 2.0F * half_height*focus_dist*v;
    }
    geometry::Ray get_ray(const float s, const float t) const
    {
        vec3 rd = lens_radius*random_in_unit_disk();
        vec3 offset = u * rd[0] + v * rd[1];;
        return geometry::Ray{origin +offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset,
                random_float(time0, time1)};
    }
};
#endif
