#ifndef LIGHT_H
#define LIGHT_H
//==============================================================================================
// Author: Guanxiong Chen
//==============================================================================================

#include "rtweekend.h"

class light {
    public:
        /* computes color at hit point due to light effects, inclduing
         * shadows, diffuse and specular effects. Uses Blinn model.
         * sets the color at hit point;
         * returns true if point is not under shadow
         */
        virtual bool compute_color(
            const hittable_list& world, const ray& r, const hit_record& rec, color& col) const = 0;
};

#endif
