#ifndef MATERIAL_H
#define MATERIAL_H
#include "../hitable/hitable.hpp"
#include "../util.hpp"
#include "texture/texture.hpp"

using vec3 = geometry::vec<float, 3>;

inline vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3{ random_float(), random_float(), random_float() } - vec3{ 1, 1, 1 };
    } while (p.square_len() >= 1.0f);
    return p;
}

inline vec3 reflect(const vec3 &v, const vec3 &n) { return v - n * 2 * (geometry::dot(n, v)); }


class material
{
  public:
    material() = default;
    material& operator=(const material &) = delete;
    material(const material &) = delete;
    virtual ~material() = default;

    virtual bool scatter(const geometry::Ray &r_in,
        const hit_record &rec,
        vec3 &atteunation,
        geometry::Ray &scattered) const = 0;

    virtual vec3 emitted([[maybe_unused]] float u, [[maybe_unused]] float v, [[maybe_unused]] const vec3 &p) const
    {
        return vec3{ 0, 0, 0 };
    }
};

class lambertian : public material
{
  public:
    lambertian(std::unique_ptr<texture> a) : albedo(std::move(a)){};
    bool scatter(const geometry::Ray &r_in,
        const hit_record &rec,
        vec3 &atteunation,
        geometry::Ray &scattered) const override
    {
        const vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = geometry::Ray(rec.p, target - rec.p, r_in.time);
        atteunation = albedo->value(0, 0, rec.p);
        return true;
    }

    std::unique_ptr<texture> albedo;
};


class metal : public material
{
  public:
    metal(const vec3 a, float f) : albedo(a)
    {
        if (f < 1)
            fuzz = f;
        else
            fuzz = 1;
    };
    bool scatter(const geometry::Ray &r_in,
        const hit_record &rec,
        vec3 &atteunation,
        geometry::Ray &scattered) const override
    {
        const vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
        scattered = geometry::Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        atteunation = albedo;
        return geometry::dot(scattered.direction(), rec.normal) > 0;
    }

    vec3 albedo;
    float fuzz;
};


inline bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{
    const vec3 uv = v.unit_vector();
    const float dt = geometry::dot(uv, n);
    const float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else
        return false;
}

class dielectric : public material
{
  public:
    float ref_index;
    dielectric(float ri) : ref_index(ri) {}
    bool scatter(const geometry::Ray &r_in,
        const hit_record &rec,
        vec3 &atteunation,
        geometry::Ray &scattered) const override
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        atteunation = vec3{ 1.0f, 1.0f, 1.0f };
        vec3 refracted{ 1.0f, 1.0f, 1.0f };
        float cosine;
        float reflect_prob;
        if (geometry::dot(r_in.direction(), rec.normal) > 0.0f)
        {
            outward_normal = -1.0f * rec.normal;
            ni_over_nt = ref_index;
            cosine = geometry::dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = sqrt(1.0f - ref_index * ref_index * (1.0f - cosine * cosine));
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_index;
            cosine = -1.0f * geometry::dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        { reflect_prob = schlick(cosine, ref_index); }
        else
        {
            reflect_prob = 1.0f;
        }
        if (random_float() < reflect_prob) { scattered = geometry::Ray{ rec.p, reflected }; }
        else
        {
            scattered = geometry::Ray{ rec.p, refracted };
        }
        return true;
    }

    float schlick(float cosine, float ref_index_temp) const
    {
        float r0 = (1.0f - ref_index_temp) / (1.0f + ref_index_temp);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * static_cast<float>(pow((1.0f - cosine), 5));
    }
};


#endif
