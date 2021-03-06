#include <iostream>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <random>

#include "../include/ray_tracer.hpp"



std::unique_ptr<hitable_list> two_perlin()
{
    auto world_list = std::make_unique<hitable_list>();
    world_list->list.emplace_back(std::make_unique<sphere>(
        vec3{0.0f,-1000.0f,0.0f},
        1000.0f,
        std::make_unique<lambertian>(std::make_unique<noise_texture>(4))));
    world_list->list.emplace_back(std::make_unique<sphere>(
        vec3{0.0f, 2.0f, 0.0f},
        2,
        std::make_unique<lambertian>(std::make_unique<noise_texture>(4))));
    return world_list;
}

std::unique_ptr<bvhNode> random_scene()
{
    std::vector<hitable_ptr> spherelist;
    spherelist.emplace_back(std::make_unique<sphere>( vec3{0.0f, -1000.0f, 0.0f}, 1000.0f,
                                                      std::make_unique<lambertian>(
                                                          std::make_unique<checker_texture>(vec3{0.9f, 0.9f, 0.9f},vec3{0.1f ,0.1f, 0.1f}))));

    for(int a=-8; a<8; a++)
    {
        for(int  b=-5; b<5; b++)
        {
            float choose_mat = random_float();
            vec3 center{a+0.9f*random_float(), 0.2f, b+0.9f*random_float()};
            if ((center - vec3{4.0f, 0.2f, 0.0f}).length() > 0.9f)
            {
                if( choose_mat <0.3f)
                    spherelist.emplace_back(std::make_unique<moving_sphere>( center, center+ vec3{0.0f, 0.5f* random_float(), 0},
                                                                             0.0f, 1.0f, 0.2f,
                                                                             std::make_unique<lambertian>(
                                                                                 std::make_unique<const_texture>(
                                                                                     vec3{random_float()*random_float(),
                                                                                          random_float()*random_float(),
                                                                                          random_float()*random_float()}))));
                else if(choose_mat < 0.5)
                {
                    spherelist.emplace_back(std::make_unique<sphere>(center, 0.2f,
                                                                     std::make_unique<metal>(vec3{0.5f*(1+random_float()),
                                                                                                  0.5f*(1+random_float()),
                                                                                                  0.5f*(1+random_float())},
                                                                         0.5f*random_float())));
                }
                else {
                    spherelist.emplace_back(std::make_unique<sphere>( center, 0.2f, std::make_unique<dielectric>(1.5f)));

                }
            }
        }
    }

    spherelist.emplace_back(std::make_unique<sphere>( vec3{0.0f,1.0f,0.0f}, 1.0f, std::make_unique<dielectric>(1.5f)));
    spherelist.emplace_back(std::make_unique<sphere>( vec3{-4.0f,1.0f,0.0f}, 1.0f, std::make_unique<dielectric>(1.5f)));
    // spherelist.emplace_back(std::make_unique<sphere>( vec3{4,1,0}, 1, std::make_unique<metal>(vec3{0.7,0.6,0.5},0.0)));
    spherelist.emplace_back(std::make_unique<sphere>( vec3{0, 12, 0}, 10,
                                                      std::make_unique<diffuse_light>(
                                                          std::make_unique<const_texture>(vec3{1.0f, 1.0f, 1.0f}))));
    auto bvh_list =std::make_unique<bvhNode>(spherelist.begin(), spherelist.end(), 0.0, 1.0);
    return  bvh_list;
}

std::unique_ptr<bvhNode> tri_scene()
{
    std::vector<hitable_ptr> spherelist;
    spherelist.emplace_back(std::make_unique<triangle>( vec3{0.0f,0.0f, 0.0f},  vec3{1.0f, 0.0f, 0.0f}, vec3{0.0f, 1.0f, 0.0f},
                                                        std::make_unique<diffuse_light>(
                                                            std::make_unique<const_texture>(vec3{10.4f, 5.9f, 10.9f}))));

    spherelist.emplace_back(std::make_unique<triangle>( vec3{0.0f,0.0f, -2.0f},  vec3{2.2f, 1.0f, -2.0f}, vec3{-1.2f, -2.0f, -0.1f},
                                                        std::make_unique<lambertian>(std::make_unique<const_texture>(vec3{0.8F, 0.5F, 0.9F}))));

         spherelist.emplace_back(std::make_unique<sphere>( vec3{0.0f, -1000.0f, 0.0f}, 1000.0f,
                                                      std::make_unique<lambertian>(
                                                          std::make_unique<checker_texture>(vec3{0.9f, 0.9f, 0.9f},vec3{0.1f ,0.1f, 0.1f}))));
    auto bvh_list = std::make_unique<bvhNode>(spherelist.begin(), spherelist.end(), 0.0f, 1.0f);
    return  bvh_list;
}

int main()
{

    constexpr auto nx = 400;
    constexpr auto ny = 300;
    constexpr auto ns = 256;

    // constexpr vec3 lookfrom{13,2,3};
    // constexpr vec3 lookat{0,0,0};
    const vec3 lookfrom{0.0f, 2.0f,9.0f};
    const vec3 lookat{0.0f,0.0f,0.0f};
    const float distance_focus = 10.0f;
    camera cam(lookfrom, lookat, vec3{0.0f,1.0f,0.0f}, 20.0f, float(nx)/float(ny), 0.0f, distance_focus, 0.0f, 1.0f);

    std::unique_ptr<hitable> world = random_scene();

    Scene sc(cam, nx, ny, std::move(world));

    auto image = sc.render(ns);
    unsigned error = lodepng::encode("img.png", image, nx, ny);

    //if there's an error, display it
    if(error)
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

    return 0;
}
