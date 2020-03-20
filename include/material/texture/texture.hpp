#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../geometry/vec.hpp"
#include <memory>
#include "perlin.hpp"

using vec3 = geometry::vec<float,3>;
class texture
{
  public:
    texture() = default;
    texture & operator=(const texture &) = delete;
    texture(const texture &) = delete;
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
    virtual ~texture() = 0;
};
texture::~texture() = default;

class const_texture : public texture
{
  public:
    vec3 colour;

    const_texture() = default;
    const_texture(const vec3 c) :colour(c){}
    vec3 value([[maybe_unused]] float u, [[maybe_unused]] float v,[[maybe_unused]]  const vec3 &p) const override
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

    vec3 value(float u, float v, const vec3 &p) const override
    {
        const float sines = sin(10 * p[0])* sin(10* p[1])* sin(10 * p[2]);
        if (sines < 0.0f)
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
    vec3 value([[maybe_unused]] float u, [[maybe_unused]] float v,[[maybe_unused]]  const vec3 &p) const override
    {
        return vec3{1.0f, 1.0f, 1.0f}* 0.5f *( 1.0f + sin(scale*p[2] + 10.0f*noise.turb(p)));
    }
    perlin noise;
    float scale;
};

void get_sphere_uv(const vec3& p, float& u, float& v) {
    float phi = atan2(p[2], p[0]);
    float theta = asin(p[1]);
    u = 1.0f - (phi + static_cast<float>(M_PI)) / (2*static_cast<float>(M_PI));
    v = (theta + static_cast<float>(M_PI)/2.0f) / static_cast<float>(M_PI);
}

class image_texture : public texture {
public:

    image_texture(std::vector<uint8_t> &pixels, int A, int B)
        : data(pixels), nx(A), ny(B) {}

     vec3 value(float u, float v,[[maybe_unused]] const vec3& p) const override
    {
        int i = static_cast<int>( u * float(nx));
        int j = static_cast<int>((1-v) * float(ny) - 0.001f);
        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > nx-1) i = nx-1;
        if (j > ny-1) j = ny-1;
        size_t index = static_cast<size_t>(3*i + 3*nx*j);
        const float r = int(data[index]  ) / 255.0f;
        const float g = int(data[index+1]) / 255.0f;
        const float b = int(data[index+2]) / 255.0f;
        return vec3{r, g, b};
    }
    std::vector<uint8_t> data;
    int nx, ny;
};



#endif
