#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec.hpp"
#include <memory>

using vec3 = geometry::vec<double,3>;
class texture
{
  public:
    virtual vec3 value(double u, double v, const vec3 &p) const = 0;
};

class const_texture : public  texture
{
  public:
    vec3 colour;

    const_texture(){}
    const_texture(const vec3 c) :colour(c){}
    virtual vec3 value(double u, double v, const vec3 &p) const
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
    checker_texture(texture * t0, texture * t1):even(t0), odd(t1)
    {
    }
    checker_texture(const vec3 &v0, const vec3 &v1):
            even(std::make_shared<const_texture>(v0)),
            odd(std::make_shared<const_texture>(v1)) {}
    virtual vec3 value(double u, double v, const vec3 &p) const
    {
        double sines = sin(10 * p[0])* sin(10* p[1])* sin(10 * p[2]);
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




#endif
