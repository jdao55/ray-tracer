#ifndef RAYT_H
#define RAYT_H

#include <iostream>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <random>
#include <taskflow/taskflow.hpp>
#include <fstream>

#include "geometry/vec.hpp"
#include "geometry/ray.hpp"
#include "geometry/camera.hpp"

#include "hitable/sphere.hpp"
#include "hitable/moving_sphere.hpp"
#include "hitable/hitable_list.hpp"
#include "hitable/triangle.hpp"
#include "hitable/bvh.hpp"


#include "float.h"
#include "util.hpp"
#include "hitable/bvh.hpp"
#include "material/material.hpp"
#include "material/light.hpp"
#include "../lodepng/lodepng.h"

using vec3 = geometry::vec<float,3>;


constexpr uint8_t tocolor(const float n)
{
    return uint8_t(256 * clamp(n, 0.0F, 0.999F));
}


inline vec3 ray_colour(const geometry::Ray &r, const hitable & world, int depth, const vec3 background= vec3{0.0f,0.0f,0.0f})
{
    hit_record rec;
    vec3 col{0.0F, 0.0F, 0.0F};
    geometry::Ray cur_ray = r;
    vec3 curr_attenuation{1.0F, 1.0F, 1.0F};
    int i=0;
    geometry::Ray scattered;
    vec3 attenuation;
    while(i<depth && world.hit(cur_ray, 0.001F, MAXFLOAT, rec))
    {
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if(!rec.mat_ptr->scatter(cur_ray, rec, attenuation, scattered))
        {
            col += emitted*curr_attenuation;
            break;
        }
        col += emitted * curr_attenuation;
        cur_ray = scattered;
        curr_attenuation = attenuation*curr_attenuation;
        i++;
    }
    // for(auto i=0;i < depth; i++)
    // {
    //     if(!world.hit(cur_ray, 0.001F, MAXFLOAT, rec)){
    //        break;
    //     }

    //     geometry::Ray scattered;
    //     vec3 attenuation;
    //     vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    //     if(!rec.mat_ptr->scatter(cur_ray, rec, attenuation, scattered))
    //     {
    //         col += emitted*curr_attenuation;;
    //         break;
    //     }
    //     col += emitted;
    //     cur_ray = scattered;
    //     curr_attenuation = attenuation*curr_attenuation;
    // }
     return col;
    // if(depth<=0){
    //     return vec3{0.0F,0.0F, 0.0F};
    // }
    // hit_record rec;
    // if(!world.hit(r, 0.001f, MAXFLOAT, rec) )
    // {

    //     return background;
    // }

    // geometry::Ray scattered;
    // vec3 atteunation;
    // vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    // if (!rec.mat_ptr->scatter(r, rec, atteunation, scattered))
    // {
    //     return emitted;
    // }
    // return emitted+ atteunation*ray_colour(scattered, world, depth-1 );
}


struct task{
    camera &cam;
    hitable &world;
    int row;
    size_t nx;
    size_t ny;
    size_t samples;
    std::vector<uint8_t> & image;

    task(
        camera &cam_,
        hitable &world_,
        int row_,
        size_t nx_,
        size_t ny_,
        size_t samples_,
        std::vector<uint8_t> & image_):
        cam(cam_), world(world_), row(row_), nx(nx_), ny(ny_), samples(samples_), image(image_){}

    void operator()() {
        std::vector<uint8_t> block_pixels;
        block_pixels.reserve(nx*4);
        for (size_t i = 0 ; i < nx ; i++)
        {
            vec3 col{0,0,0};
            for(size_t s=0; s < samples; s++)
            {
                const float u = (static_cast<float>(i)+random_float())/float(nx);
                const float v = (static_cast<float>(row)+random_float())/float(ny);
                const geometry::Ray r=cam.get_ray(u,v);

                col += ray_colour(r, world, 50);

            }
            col /= float(samples);
            block_pixels.push_back(tocolor(col[0])); //red
            block_pixels.push_back(tocolor(col[1]));//green
            block_pixels.push_back(tocolor(col[2]));//blue
            block_pixels.push_back(255);//alpha
        }
        auto start_index = static_cast<long int>(((ny-1-static_cast<size_t>(row))*nx*4));
        std::copy(block_pixels.begin(), block_pixels.end(), image.begin() + start_index);
    }
};

struct Scene {
    camera cam;
    size_t x;
    size_t y;
    std::unique_ptr<hitable> objects;
    Scene(camera c, size_t _x, size_t _y, std::unique_ptr<hitable> obj) :
        cam(c), x(_x), y(_y),
        objects(std::move(obj))
    {}
    Scene(camera c, size_t _x, size_t _y):cam(c), x(_x), y(_y) {}
    std::vector<uint8_t> render(size_t samples)
    {
        tf::Executor executor;
        tf::Taskflow taskflow;

        std::vector<uint8_t> image(x*y*4);

        std::vector<task> job_list;
        job_list.reserve(y);
        for(auto row = static_cast<int>(y)-1; row >= 0; row -= 1)
        {
            job_list.emplace_back(cam, *objects, row, x, y, samples, image);
        }
        taskflow.parallel_for(job_list.begin(), job_list.end(),
                              [](auto& job){job();});
        executor.run(taskflow).wait();
        return image;
    }
};

#endif
