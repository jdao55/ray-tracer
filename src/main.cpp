#include <iostream>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <random>
#include "geometry/vec.hpp"
#include "geometry/ray.hpp"
#include "geometry/camera.hpp"
#include <fstream>
#include "hitable/sphere.hpp"
#include "hitable/moving_sphere.hpp"
#include "hitable/hitable_list.hpp"
#include "float.h"
#include "util.hpp"
#include "hitable/bvh.hpp"
#include "material/material.hpp"
#include "../lodepng/lodepng.h"

using vec3 = geometry::vec<float,3>;
using hitable_ptr = std::shared_ptr<hitable>;


uint8_t tocolor(const float n)
{
    return uint8_t(255.99*sqrt(n));
}


std::unique_ptr<hitable_list> two_perlin()
{
    auto world_list = std::make_unique<hitable_list>();
    world_list->list.emplace_back(std::make_shared<sphere>(
        vec3{0,-1000,0},
        1000,
        std::make_shared<lambertian>(std::make_unique<noise_texture>(4))));
    world_list->list.emplace_back(std::make_shared<sphere>(
        vec3{0, 2, 0},
        2,
        std::make_shared<lambertian>(std::make_unique<noise_texture>(4))));
    return world_list;
}


vec3 colour(const geometry::Ray &r, hitable & world, int depth, const vec3 = vec3{0,0,0})
{
    hit_record rec;
    if( world.hit(r, 0.001, MAXFLOAT, rec) )
    {
        geometry::Ray scattered;
        vec3 atteunation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, atteunation, scattered))
        {
            return atteunation*colour(scattered, world, depth+1 );
        }
        else
        {
            return vec3{0,0,0};
        }

    }
    else
    {
        const vec3 unit_dir = (r.direction()).unit_vector();
        const float t = 0.5 * (unit_dir[1] + 1.0);
        return (1.0-t)*vec3{1.0, 1.0, 1.0} + t *vec3{0.5, 0.7, 1.0};
    }
}

std::unique_ptr<bvhNode> random_scene()
{
    std::vector<hitable_ptr> spherelist;

    spherelist.emplace_back(std::make_shared<sphere>( vec3{0, -1000, 0}, 1000,
                                                      std::make_shared<lambertian>(
                                                          std::make_unique<checker_texture>(vec3{0.9, 0.9, 0.9},vec3{0.1 ,0.1, 0.1}))));
    for(int a=-10; a<10; a++)
    {
        for(int  b=-10; b<10; b++)
        {
            float choose_mat = random_float();
            vec3 center{a+0.9f*random_float(), 0.2, b+0.9f*random_float()};
            if ((center - vec3{4,0.2,0}).length() > 0.9)
            {
                if( choose_mat <0.8)
                    spherelist.emplace_back(std::make_shared<moving_sphere>( center, center+ vec3{0, 0.5f* random_float(), 0},
                                                                             0.0, 1.0, 0.2,
                                                                             std::make_shared<lambertian>(
                                                                                 std::make_unique<const_texture>(
                                                                                     vec3{random_float()*random_float(),
                                                                                          random_float()*random_float(),
                                                                                          random_float()*random_float()}))));
                else if(choose_mat < 0.95)
                {
                    spherelist.emplace_back(std::make_shared<sphere>(center, 0.2,
                                                                     std::make_shared<metal>(vec3{0.5f*(1+random_float()),
                                                                                                  0.5f*(1+random_float()),
                                                                                                  0.5f*(1+random_float())},
                                                                         0.5*random_float())));
                }
                else {
                    spherelist.emplace_back(std::make_shared<sphere>( center, 0.2, std::make_shared<dielectric>(1.5)));

                }
            }
        }
    }

    spherelist.emplace_back(std::make_shared<sphere>( vec3{0,1,0}, 1, std::make_shared<dielectric>(1.5)));
    spherelist.emplace_back(std::make_shared<sphere>( vec3{-4,1,0}, 1, std::make_shared<dielectric>(1.5)));
    spherelist.emplace_back(std::make_shared<sphere>( vec3{4,1,0}, 1, std::make_shared<metal>(vec3{0.7,0.6,0.5},0.0)));
    auto bvh_list =std::make_unique<bvhNode>(spherelist.begin(), spherelist.end(),0.0, 1.0);
    return  bvh_list;
}

void process_block(const camera &cam,
                   hitable & world,
                   const int start_row,
                   const int num_rows,
                   const size_t nx, const size_t ny,
                   const size_t samples,
                   std::vector<uint8_t> & image)
{
    std::vector<uint8_t> block_pixels;
    block_pixels.reserve(nx*4*num_rows);
    for (int j = start_row ; j>=start_row - num_rows && j >= 0 ; j--)
    {

        for (auto i = 0 ; size_t(i) < nx ; i++)
        {

            vec3 col{0,0,0};
            for(auto s=0; size_t(s) <samples;s++)
            {
                const float u = float(i+random_float())/float(nx);
                const float v = float(j+random_float())/float(ny);
                const geometry::Ray r=cam.get_ray(u,v);

                col += colour(r, world, 0);

            }
            col /= float(samples);
            block_pixels.push_back(tocolor(col[0])); //red
            block_pixels.push_back(tocolor(col[1]));//green
            block_pixels.push_back(tocolor(col[2]));//blue
            block_pixels.push_back(255);//alpha
        }
    }
    std::copy(block_pixels.begin(), block_pixels.end(), image.begin()+(ny-1-start_row)*nx*4);
}

int main()
{


    constexpr auto nx = 800;
    constexpr  auto ny = 500;
    constexpr auto ns = 8;
    size_t num_threads = 12;
    size_t rows_thread = ny / num_threads;

    constexpr vec3 lookfrom{13,2,3};
    constexpr vec3 lookat{0,0,0};
    constexpr float distance_focus = 10.0;
    const camera cam(lookfrom, lookat, vec3{0,1,0}, 20, float(nx)/float(ny), 0.00, distance_focus, 0.0, 1.0);

    std::unique_ptr<hitable> world = random_scene();

    std::vector<uint8_t> image(nx*ny*4);

#pragma omp parallel for
    for(auto start_row=ny-1; start_row>=0; start_row-=rows_thread)
    {
        process_block(cam, *world, start_row, rows_thread, nx, ny, ns, image);
    }

    unsigned error = lodepng::encode("img.png", image, nx, ny);

    //if there's an error, display it
    if(error)
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}
