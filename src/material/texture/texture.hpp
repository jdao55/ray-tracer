#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../geometry/vec.hpp"
#include <memory>
#include "perlin.hpp"

using vec3 = geometry::vec<float,3>;
class texture
{
  public:
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};

class const_texture : public  texture
{
  public:
    vec3 colour;

    const_texture(){}
    const_texture(const vec3 c) :colour(c){}
    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        return colour;
    }
};

class checker_texture : public texture
{
  public:
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;


    checker_texture(){}
    checker_texture(texture * t0, texture * t1):even(t0), odd(t1){}
    checker_texture(const vec3 &v0, const vec3 &v1):
            even(std::make_shared<const_texture>(v0)),
            odd(std::make_shared<const_texture>(v1)) {}

    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        const float sines = sin(10 * p[0])* sin(10* p[1])* sin(10 * p[2]);
        if (sines <0)
        {
            return odd->value(u, v,p);
        }
        else
        {
            return even->value(u, v, p);
        }
    }
};

class noise_texture : public texture
{
  public:
    noise_texture(float sc):scale(sc){};
    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        return vec3{1,1,1}*0.5*( 1 + sin(scale*p[2] + 10*noise.turb(p)));
    }
    perlin noise;
    float scale;
};

#endif
