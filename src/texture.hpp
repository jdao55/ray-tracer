#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec.hpp"

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
    const_texture(vec3 c) :colour(c){}
    virtual vec3 value(double u, double v, const vec3 &p) const
    {
        return colour;
    }
};


#endif
