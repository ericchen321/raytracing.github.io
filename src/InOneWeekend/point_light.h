#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
//==============================================================================================
// Author: Guanxiong Chen
//==============================================================================================

#include "rtweekend.h"
#include "light.h"
#include "hittable_list.h"
#include "color.h"

class point_light : public light {
    public:
        point_light() {};

        point_light(color color_light, point3 pos_light) {
            col_light = color_light;
            pos = pos_light;
        }

        /* get the unitary light vector, from fragment to light source
         */
        vec3 get_light_vector(point3 pos_frag) const {
            return unit_vector(pos - pos_frag);
        }

        virtual bool compute_color(
            const hittable_list& world, const hit_record& rec, color& col) const override {
            ray shadow_ray = ray(rec.p, get_light_vector(rec.p));
            hit_record shadow_ray_hit_rec;
            bool if_under_shadow = world.hit(shadow_ray, 0.001, infinity, shadow_ray_hit_rec);
            if (if_under_shadow) {
                // if hit point is under shadow, set col to black and return false
                col = color(0,0,0);
                return false;
            }
            else {
                // if hit point is not under shadow, set col to color of light,
                // and return true
                col = col_light;
                return true;
            }
        }

    public:
        color col_light;
        point3 pos;
};

#endif
