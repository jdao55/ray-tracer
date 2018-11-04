#ifndef BVH_H
#define BVH_H

#include "hitable.hpp"
#include <vector>
#include <algorithm>
using vec3 = geometry::vec<double,3>;
using ray = geometry::Ray;
using hitable_ptr = std::shared_ptr<hitable>;

inline bool box_cmp(const hitable & a, const hitable &b, int dim);

inline auto box_x_compare (const hitable_ptr &a, const hitable_ptr & b)
{
    aabb box_left, box_right;

    if(!a->bounding_box(0,0, box_left) || !b->bounding_box(0,0, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";

    return  ( box_right._min[0] - box_left._min[0] < 0.0  );
};

inline auto box_y_compare (const hitable_ptr &a, const hitable_ptr & b)
{
    aabb box_left, box_right;

    if(!a->bounding_box(0,0, box_left) || !b->bounding_box(0,0, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";

    return  ( box_right._min[1] - box_left._min[1] < 0.0  );
};

inline auto box_z_compare (const hitable_ptr &a, const hitable_ptr & b)
{
    aabb box_left, box_right;

    if(!a->bounding_box(0,0, box_left) || !b->bounding_box(0,0, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";

    return  ( box_right._min[2] - box_left._min[2] < 0.0  );
};

class bvhNode : public hitable
{
  private:
  public:
    hitable_ptr left, right;
    aabb box;
    bvhNode(){};

    using hitable_iter = std::vector<hitable_ptr>::iterator;
    bvhNode (hitable_iter list_begin,  hitable_iter list_end, double time0, double time1)
    {
        int n = std::distance(list_begin, list_end);
        int axis = int(3*drand48());
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

    bool hit(const ray& r, double tmin, double tmax, hit_record & rec) const
    {
        if (box.hit(r, tmin, tmax)) {
            hit_record left_rec, right_rec;
            bool hit_left = left->hit(r, tmin, tmax, left_rec);
            bool hit_right = right->hit(r, tmin, tmax, right_rec);
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

    bool bounding_box(double t0, double t1, aabb& box1) const {
        box1=box;
        return true;
    }


};
#endif
