#ifndef AABB_H
#define AABB_H

#include "../geometry/vec.hpp"
#include "../geometry/ray.hpp"

using vec3 = geometry::vec<float, 3>;
struct aabb
{
    vec3 _min;
    vec3 _max;

    aabb() = default;
    aabb(const vec3 a, const vec3 b) : _min(a), _max(b) {}

    vec3 min() const {return _min; }
    vec3 max() const {return _max; }

    bool hit(const geometry::Ray &r, float tmin, float tmax) const
    {
        for (size_t a = 0; a < 3; a++)
        {
            const float t0 =
                std::min((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
            const float t1 =
                std::min((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
            tmin = std::min(t0, tmin);
            tmax = std::max(t1, tmax);
            if (tmax <= tmin) return false;
        }
        return true;
    }
    double area() const
    {
        auto a = _max[0] - _min[0];
        auto b = _max[1] - _min[1];
        auto c = _max[2] - _min[2];
        return 2 * (a * b + b * c + c * a);
    }

    int longest_axis() const
    {
        auto a = _max[0] - _min[0];
        auto b = _max[1] - _min[1];
        auto c = _max[2] - _min[2];
        if (a > b && a > c)
            return 0;
        else if (b > c)
            return 1;
        else
            return 2;
    }
};

aabb surrounding_box(aabb box0, aabb box1) {
    vec3 small{fmin(box0.min()[0], box1.min()[0]),
               fmin(box0.min()[1], box1.min()[1]),
               fmin(box0.min()[2], box1.min()[2])};

    vec3 big  {fmax(box0.max()[0], box1.max()[0]),
               fmax(box0.max()[1], box1.max()[1]),
               fmax(box0.max()[2], box1.max()[2])};

    return aabb{small,big};
}

#endif
