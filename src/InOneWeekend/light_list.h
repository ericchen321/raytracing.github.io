#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H
//==============================================================================================
// Author: Guanxiong Chen
//==============================================================================================

#include "rtweekend.h"

#include "hittable_list.h"
#include "light.h"

#include <memory>
#include <vector>


class light_list : public light  {
    public:
        light_list() {}
        light_list(shared_ptr<light> light) { add(light); }

        void clear() { lights.clear(); }
        void add(shared_ptr<light> light) { lights.push_back(light); }

        virtual bool compute_color(
            const hittable_list& world, const hit_record& rec, color& col) const override;
            
    public:
        std::vector<shared_ptr<light>> lights;
};


bool light_list::compute_color(const hittable_list& world, const hit_record& rec, color& col) const {
    color temp_col;
    auto is_under_light = false;

    for (const auto& light : lights) {
        if (light->compute_color(world, rec, temp_col)) {
            is_under_light = true;
            col = col + temp_col;
        }
    }

    return is_under_light;
}


#endif
