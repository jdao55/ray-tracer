#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.hpp"
#include <memory>
#include <vector>

class hitable_list :public hitable
{
  public:
    std::vector<std::unique_ptr<hitable>> list;
    hitable_list() {}
    bool hit(const geometry::Ray& r, float t_min, float t_max, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        float closet_so_far =t_max;
        for(const auto & elem: list)
        {
            if(elem->hit(r, t_min, closet_so_far, temp_rec))
            {
                hit_anything=true;
                closet_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        if (list.empty()) return false;

        aabb temp_box;
        bool first_box = true;

        for (const auto& object : list) {
            if (!object->bounding_box(t0, t1, temp_box)) return false;
            box = first_box ? temp_box : surrounding_box(box, temp_box);
            first_box = false;
        }

        return true;

    }
};
#endif
