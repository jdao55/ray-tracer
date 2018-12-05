#ifndef AABB_H
#define AABB_H

#include "../geometry/vec.hpp"
#include "../geometry/ray.hpp"

using vec3 = geometry::vec<float,3>;
inline float ffmax(float a, float b){return a>b?a:b;}
inline float ffmin(float a, float b){return a<b?a:b;}
struct aabb
{
    vec3 _min, _max;

    aabb(){};
    aabb(const vec3 a, const vec3 b): _min(a), _max(b){}

    bool hit(const geometry::Ray &r, float tmin, float tmax) const {
        for (auto a =0; a<3; a++)
        {
            const float t0 = ffmin((_min[a] - r.origin()[a])/ r.direction()[a],
                              (_max[a] - r.origin()[a])/ r.direction()[a] );
            const float t1 = ffmax((_min[a] - r.origin()[a])/ r.direction()[a],
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
