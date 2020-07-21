#ifndef BVH_H
#define BVH_H

#include "hitable.hpp"
#include "../util.hpp"
#include <memory>
#include <vector>
#include <algorithm>
using vec3 = geometry::vec<float,3>;
using ray = geometry::Ray;
using hitable_ptr = std::unique_ptr<hitable>;

enum axis{
    x = 0,
    y = 1,
    z = 2
};
inline bool box_cmp(const hitable_ptr& a, const hitable_ptr &b,const  axis dim){
    aabb box_left;
    aabb box_right;

    if(!a->bounding_box(0.0F, 0.0F, box_left) || !b->bounding_box(0.0F, 0.0F, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";

    return  ( box_right._min[dim] - box_left._min[dim] < 0.0f  );
}


class bvhNode : public hitable
{
  private:
  public:
    hitable_ptr left=nullptr;
    hitable_ptr right=nullptr;
    aabb box;
    bvhNode(){};

    using hitable_iter = std::vector<hitable_ptr>::iterator;
    bvhNode (hitable_iter list_begin,  hitable_iter list_end, float time0, float time1)
    {
        const auto n = std::distance(list_begin, list_end);
        const axis dim = static_cast<axis>(random_int(0,2));

        switch (n){
            case 1:
                left = std::move(*list_begin);
                right = nullptr;
                break;
            case 2:
                if(box_cmp(*list_begin, *(list_begin+1), dim))
                {
                    left = std::move(*list_begin);
                    right =  std::move(*(list_begin+1));
                }
                else {
                    left =std::move( *(list_begin+1));
                    right =std::move(*list_begin);
                }
                break;
            default:
                std::sort(list_begin, list_end, [dim](const auto &a, const auto &b){
                                                    return box_cmp(a,b,dim);
                                                });
                left = std::make_unique<bvhNode>(list_begin, list_begin+n/2, time0, time1);
                right = std::make_unique<bvhNode>(list_begin+n/2, list_end, time0, time1);
                break;
        };
        aabb box_left;
        aabb box_right;
        left->bounding_box(time0,time1, box_left);
        if(right){
            right->bounding_box(time0,time1, box_right);
        }
        else{
            box_right=box_left;
        }

        box = surrounding_box(box_left, box_right);
    }

    bool hit(const ray& r, const float tmin, const float tmax, hit_record & rec) const override
    {
        if (!box.hit(r, tmin, tmax))
        return false;

        bool hit_left = left?left->hit(r, tmin, tmax, rec):false;
        bool hit_right = right? right->hit(r, tmin, hit_left ? rec.t : tmax, rec):false;

        return hit_left || hit_right;
    }

    bool bounding_box([[maybe_unused]] float t0, [[maybe_unused]] float t1, aabb& box1) const override
    {
        box1=box;
        return true;
    }


};
#endif
