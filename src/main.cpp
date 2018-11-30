#include <iostream>
#include <cstdint>
#include <utility>
#include <random>
#include "vec.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include <fstream>
#include "sphere.hpp"
#include  "moving_sphere.hpp"
#include "hitable_list.hpp"
#include "float.h"
#include "bvh.hpp"
#include "material.hpp"

using vec3 = geometry::vec<double,3>;
using hitable_ptr = std::shared_ptr<hitable>;


std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<double> rand_float(0.0,1.0);


inline uint tocolor(float n)
{
    return uint(255.99*sqrt(n));
}

inline void print_vec(const vec3 &v)
{
     std::cout <<v[0]<< " "<< v[1] << " " <<v[2] <<"\n";
}


inline vec3 colour(const geometry::Ray &r, hitable & world, int depth)
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
        vec3 unit_dir = (r.direction()).unit_vector();
        double t = 0.5 * (unit_dir[1] + 1.0);
        return (1.0-t)*vec3{1.0, 1.0, 1.0} + t *vec3{0.5, 0.7, 1.0};
    }
}

std::unique_ptr<bvhNode> random_scene()
{
    std::vector<hitable_ptr> spherelist;

    spherelist.emplace_back(std::make_shared<sphere>( vec3{0, -1000, 0}, 1000,std::make_shared<lambertian>(std::make_unique<checker_texture>(vec3{0.9, 0.9, 0.9},vec3{0.1 ,0.1, 0.1}))));
    for( auto a= -10; a<10; a++)
    {
        for(auto b= -10; b<10; b++)
        {
            double choose_mat = rand_float(e2);
            vec3 center{a+0.9*rand_float(e2), 0.2, b+0.9*rand_float(e2)};
            if ((center - vec3{4,0.2,0}).length() > 0.9)
            {
                if( choose_mat <0.8)
                    spherelist.emplace_back(std::make_shared<moving_sphere>( center, center+ vec3{0, 0.5* rand_float(e2), 0}, 0.0, 1.0, 0.2,
                                                                             std::make_shared<lambertian>(
                                                                                 std::make_unique<const_texture>(
                                                                                     vec3{rand_float(e2)*rand_float(e2),
                                                                                          rand_float(e2)*rand_float(e2),
                                                                                          rand_float(e2)*rand_float(e2)}))));
                else if( choose_mat < 0.95)
                {
                    spherelist.emplace_back(std::make_shared<sphere>(center, 0.2,
                                                                            std::make_shared<metal>(vec3{0.5*(1+rand_float(e2)),
                                                                                                         0.5*(1+rand_float(e2)),
                                                                                                         0.5*(1+rand_float(e2))},
                                                                                0.5*rand_float(e2))));
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

int main()
{
    constexpr long int nx = 1000;
    constexpr long int ny = 800;
    constexpr long int ns = 20;
    std::ofstream out_file("img.ppm");
    out_file<< "P3\n"<< nx <<" "<< ny<< "\n255\n";



    vec3 lookfrom{13,2,3};
    vec3 lookat{0,0,0};
    double distance_focus = 10.0;
    camera cam(lookfrom, lookat, vec3{0,1,0}, 20, double(nx)/double(ny), 0.00, distance_focus, 0.0, 1.0);

    std::unique_ptr<bvhNode> world = random_scene();

    for (auto j = ny-1 ; j >= 0 ; j--)
    {

        for (auto i = 0 ; i < nx ; i++)
        {
            vec3 col{0,0,0};
            #pragma omp parallel for num_threads(10)
            for(auto s=0; s<ns;s++)
            {
                double u = double(i+rand_float(e2))/double(nx);
                double v = double(j+rand_float(e2))/double(ny);
                geometry::Ray r=cam.get_ray(u,v);

                auto  c = colour(r, *world, 0);
                #pragma omp critical
                {
                    col += c;
                }
            }
            int red,g,b;
            col /= double(ns);
            red=tocolor(col[0]);
            g=tocolor(col[1]);
            b=tocolor(col[2]);

            out_file<<red<<" "<<g<<" "<<b<<"\n";
        }

    }
    out_file.close();
}
