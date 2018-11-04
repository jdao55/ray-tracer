#ifndef AABB_H
#define AABB_H

#include "vec.hpp"
#include "ray.hpp"

using vec3 = geometry::vec<double,3>;
inline double ffmax(double a, double b){return a>b?a:b;}
inline double ffmin(double a, double b){return a<b?a:b;}
struct aabb
{
    vec3 _min, _max;

    aabb(){};
    aabb(const vec3 &a, const vec3 &b): _min(a), _max(b){}

    bool hit(const geometry::Ray &r, float tmin, float tmax) const {
        for (auto a =0; a<3; a++)
        {
            double t0 = ffmin((_min[a] - r.origin()[a])/ r.direction()[a],
                              (_max[a] - r.origin()[a])/ r.direction()[a] );
            double t1 = ffmax((_min[a] - r.origin()[a])/ r.direction()[a],
                              (_max[a] - r.origin()[a])/ r.direction()[a] );
            tmin = ffmin(t0, tmin);
            tmax = ffmax(t1, tmax);
            if (tmax <= tmin)
                return false;

        }
        return true;
    }

};

#endif
