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
    virtual vec3 value([[maybe_unused]] float u, [[maybe_unused]] float v,[[maybe_unused]]  const vec3 &p) const
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
    virtual vec3 value([[maybe_unused]] float u, [[maybe_unused]] float v,[[maybe_unused]]  const vec3 &p) const
    {
        return vec3{1,1,1}*0.5*( 1 + sin(scale*p[2] + 10*noise.turb(p)));
    }
    perlin noise;
    float scale;
};

void get_sphere_uv(const vec3& p, float& u, float& v) {
    float phi = atan2(p[2], p[0]);
    float theta = asin(p[1]);
    u = 1-(phi + M_PI) / (2*M_PI);
    v = (theta + M_PI/2) / M_PI;
}

class image_texture : public texture {
public:
    image_texture() {}
    image_texture(std::vector<uint8_t> &pixels, int A, int B)
        : data(pixels), nx(A), ny(B) {}
    virtual vec3 value(float u, float v, const vec3& p) const;
    std::vector<uint8_t> data;
    int nx, ny;
};

vec3 image_texture::value(float u, float v, const vec3& p) const {
    int i = (  u) * nx;
    int j = (1-v) * ny - 0.001;
    if (i < 0) i = 0;
    if (j < 0) j = 0;
    if (i > nx-1) i = nx-1;
    if (j > ny-1) j = ny-1;
    float r = int(data[3*i + 3*nx*j]  ) / 255.0;
    float g = int(data[3*i + 3*nx*j+1]) / 255.0;
    float b = int(data[3*i + 3*nx*j+2]) / 255.0;
    return vec3{r, g, b};
}

#endif
