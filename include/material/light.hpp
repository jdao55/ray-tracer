#ifndef LIGHT_H
#define LIGHT_H
#include "material.hpp"
#include "../geometry/ray.hpp"
#include "texture/texture.hpp"

class diffuse_light : public material
{
  public:
    diffuse_light(std::unique_ptr<texture> a) : emit(std::move(a)) {}
    bool scatter([[maybe_unused]] const geometry::Ray &r_in,
        [[maybe_unused]] const hit_record &rec,
        [[maybe_unused]] vec3 &attenuation,
        [[maybe_unused]] geometry::Ray &scattered) const override
    {
        return false;
    }
    vec3 emitted(float u, float v, const vec3 &p) const override { return emit->value(u, v, p); }
    std::unique_ptr<texture> emit;
};
#endif
