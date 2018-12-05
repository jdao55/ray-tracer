#ifndef MATERIAL_H
#define MATERIAL_H
#include "../hitable/hitable.hpp"
#include "texture/texture.hpp"

using vec3 = geometry::vec<float,3>;

inline vec3 random_in_unit_sphere() {
    vec3 p;
    do
    {
        p = 2.0*vec3{float(drand48()),float( drand48()), float(drand48())} -vec3{1,1,1};
    } while (p.square_len() >=1.0);
    return p;
}

inline vec3 reflect (const vec3 &v, const vec3 &n)
{
  return v - n*2*(n.dot(v));
}

class material{
 public: virtual bool scatter(const geometry::Ray &r_in, const hit_record & rec, vec3 & atteunation, geometry::Ray& scattered) const =0;
};

class lambertian : public material {
 public:
    lambertian(std::unique_ptr<texture> a): albedo(std::move(a)){};
    virtual bool scatter(const geometry::Ray &r_in, const hit_record & rec, vec3 & atteunation, geometry::Ray& scattered) const {
        const vec3 target = rec.p + rec.normal +  random_in_unit_sphere();
        scattered = geometry::Ray(rec.p, target - rec.p, r_in.time);
        atteunation = albedo->value(0,0,  rec.p);
    return true;
  }

    std::unique_ptr<texture> albedo;
};

class metal : public material {
  public:
    metal(const vec3 a, float f): albedo(a){ if (f < 1) fuzz = f; else fuzz = 1; };
    virtual bool scatter(const geometry::Ray &r_in, const hit_record & rec, vec3 & atteunation, geometry::Ray& scattered) const {
        const vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
        scattered = geometry::Ray(rec.p, reflected+fuzz*random_in_unit_sphere());
        atteunation = albedo;
        return (scattered.direction().dot(rec.normal)) > 0;
    }

    vec3 albedo;
    float fuzz;
};


bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    const vec3 uv = v.unit_vector();
    const float dt = uv.dot(n);
    const float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt*dt);
    if (discriminant >0)
    {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;

}

class dielectric : public material
{
  public:
    float ref_index;
    dielectric(float ri):ref_index(ri){}
    virtual bool scatter(const geometry::Ray &r_in, const hit_record& rec, vec3& atteunation, geometry::Ray& scattered) const
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        atteunation=vec3{1.0, 1.0, 1.0};
        vec3 refracted;
        float cosine;
        float reflect_prob;
        if(r_in.direction().dot(rec.normal) > 0)
        {
            outward_normal = -1*rec.normal;
            ni_over_nt=ref_index;
            cosine = r_in.direction().dot(rec.normal)/ r_in.direction().length();
            cosine = sqrt(1-ref_index*ref_index*(1-cosine*cosine));
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0/ref_index;
            cosine = -1* r_in.direction().dot(rec.normal)/ r_in.direction().length();
        }
        if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            reflect_prob = schlick(cosine, ref_index);
        }
        else
        {
            reflect_prob=1.0;
        }
        if(drand48() < reflect_prob)
        {
            scattered = geometry::Ray{rec.p, reflected};
        }
        else{
            scattered = geometry::Ray{rec.p, refracted};
        }
        return true;
    }

    float schlick(float cosine, float ref_index) const
    {
        float r0 = (1-ref_index) / (1+ref_index);
        r0 = r0*r0;
        return r0 +(1-r0)*pow((1-cosine),5);
    }

};


#endif
