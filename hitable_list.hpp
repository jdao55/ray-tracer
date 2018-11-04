#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.hpp"
#include <memory>
#include <vector>

class hitable_list :public hitable
{
  public:
    std::vector<std::shared_ptr<hitable>> list;
    hitable_list() {}
    bool hit(const geometry::Ray& r, double t_min, double t_max, hit_record &rec) const
    {
        hit_record temp_rec;
        bool hit_anything = false;
        double closet_so_far =t_max;
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
    bool bounding_box(double t0, double t1, aabb& box) const
    {
        if (list.size()<1)
            return false;
        aabb temp_box;
        bool first_true = list[0]->bounding_box(t0, t1, temp_box);
        if (!first_true)
        {
            return true;
        }
        else
        {
            box  = temp_box;
        }
        for (auto i =0; i<list.size(); i++)
        {
            if(list[0]->bounding_box(t0, t1, temp_box))
            {
                box = surrounding_box(box, temp_box);
            }
            else
                return false;
        }
        return true;

    }
};
#endif
