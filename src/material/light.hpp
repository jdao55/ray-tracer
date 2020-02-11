#ifndef LIGHT_H
#define LIGHT_H
#include "material.hpp"
#include "../geometry/ray.hpp"

class diffuse_light : public material {
public:
    diffuse_light(texture *a) : emit(a) {}
    virtual bool scatter(const  geometry::Ray&r_in, const hit_record& rec,
                         vec3& attenuation,  geometry::Ray & scattered) const { return false; }
    virtual vec3 emitted(float u, float v, const vec3& p) const {
        return emit->value(u, v, p);
    }
    texture *emit;
};
#endif
