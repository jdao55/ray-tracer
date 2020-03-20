#ifndef BVH_H
#define BVH_H

#include "hitable.hpp"
#include "../util.hpp"
#include <vector>
#include <algorithm>
using vec3 = geometry::vec<float,3>;
using ray = geometry::Ray;
using hitable_ptr = std::shared_ptr<hitable>;

inline bool box_cmp(const hitable & a, const hitable &b, int dim);

inline auto box_x_compare (const hitable_ptr &a, const hitable_ptr & b)
{
    aabb box_left, box_right;

    if(!a->bounding_box(0.0f, 0.0f, box_left) || !b->bounding_box(0.0f, 0.0f, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";

    return  ( box_right._min[0] - box_left._min[0] < 0.0f  );
}

inline auto box_y_compare (const hitable_ptr &a, const hitable_ptr & b)
{
    aabb box_left, box_right;

    if(!a->bounding_box(0.0f,0.0f, box_left) || !b->bounding_box(0.0f,0.0f, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";

    return  ( box_right._min[1] - box_left._min[1] < 0.0f  );
}

inline auto box_z_compare (const hitable_ptr &a, const hitable_ptr & b)
{
    aabb box_left, box_right;

    if(!a->bounding_box(0.0f,0.0f, box_left) || !b->bounding_box(0.0f,0.0f, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";

    return  ( box_right._min[2] - box_left._min[2] < 0.0f  );
}

class bvhNode : public hitable
{
  private:
  public:
    hitable_ptr left, right;
    aabb box;
    bvhNode(){};

    using hitable_iter = std::vector<hitable_ptr>::iterator;
    bvhNode (hitable_iter list_begin,  hitable_iter list_end, float time0, float time1)
    {
        const auto n = std::distance(list_begin, list_end);
        const size_t axis = static_cast<size_t>(3.0f*random_float());
        if (axis == 0)
            std::sort(list_begin, list_end, box_x_compare);
        else if (axis == 1){
            std::sort(list_begin, list_end, box_y_compare);
        }
        else
            std::sort(list_begin, list_end, box_z_compare);

        if (n == 1) {
            left = right = *list_begin;
        }
        else if (n == 2) {
            left = *(list_begin);
            right = *(list_begin+1);
        }
        else {
            left = std::make_shared<bvhNode>(list_begin, list_begin+n/2, time0, time1);
            right = std::make_shared<bvhNode>(list_begin+n/2, list_end, time0, time1);
        }

        aabb box_left, box_right;
        if(!left->bounding_box(time0,time1, box_left) || !right->bounding_box(time0,time1, box_right))
            std::cerr << "no bounding box in bvh_node constructor\n";
        box = surrounding_box(box_left, box_right);
    }

    bool hit(const ray& r, const float tmin, const float tmax, hit_record & rec) const override
    {
        if (box.hit(r, tmin, tmax)) {
            hit_record left_rec;
            hit_record right_rec;
            const bool hit_left = left->hit(r, tmin, tmax, left_rec);
            const bool hit_right = right->hit(r, tmin, tmax, right_rec);
            if (hit_left && hit_right) {
                if (left_rec.t < right_rec.t)
                    rec = left_rec;
                else
                    rec = right_rec;
                return true;
            }
            else if (hit_left) {
                rec = left_rec;
                return true;
            }
            else if (hit_right) {
                rec = right_rec;
                return true;
            }
            else
                return false;
        }
        else return false;
    }

    bool bounding_box([[maybe_unused]] float t0, [[maybe_unused]] float t1, aabb& box1) const override
    {
        box1=box;
        return true;
    }


};
#endif
