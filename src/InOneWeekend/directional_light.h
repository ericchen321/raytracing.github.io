#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H
//==============================================================================================
// Author: Guanxiong Chen
//==============================================================================================

#include "rtweekend.h"
#include "light.h"
#include "hittable_list.h"
#include "color.h"
#include <algorithm> 

class directional_light : public light {
    public:
        directional_light() {};

        directional_light(color color_light, vec3 dir_light) {
            col_light = color_light;
            dir = dir_light;
        }

        /* get the unitary light vector, starting from fragment
         */
        vec3 get_light_vector() const {
            return unit_vector(-1.0 * dir);
        }

        virtual bool compute_color(
            const hittable_list& world, const ray& r, const hit_record& rec, color& col
        ) const override {
            ray shadow_ray = ray(rec.p, get_light_vector());
            hit_record shadow_ray_hit_rec;
            bool if_under_shadow = world.hit(shadow_ray, 0.001, infinity, shadow_ray_hit_rec);
            if (if_under_shadow) {
                // if hit point is under shadow, set col to black and return false
                col = color(0,0,0);
                return false;
            }
            else {
                // if hit point is not under shadow, set col based on Phong-Blinn model,
                // and return true
                // modifed from shading assignment
                vec3 light_vec = get_light_vector();
                vec3 normal_vec = unit_vector(rec.normal);
                
                // compute diffuse component
                double lambertian = std::max(dot(normal_vec, light_vec), 0.0);
                // TODO: allow passing in diffuse coefficient
                double k_diffuse = 0.8;
                color col_material, col_diffuse;
                if (rec.mat_ptr->get_color(col_material)) {
                    // sets diffuse component for lambertian or metal
                    col_diffuse = k_diffuse*lambertian*col_light*col_material;
                }
                else {
                    // otherwise no diffuse component
                    col_diffuse = color(0,0,0);
                }                
                
                // compute specular component
                // TODO: allow passing in specular coefficient and shininess
                double k_specular = 0.8;
                double shininess = 10.0;
                double spec_amount;
                if (lambertian > 0.0) {
                    // compute view vector
                    vec3 view_vec = unit_vector(r.origin()-rec.p);
                    // compute half vector
                    vec3 half_vec = 0.5 * (light_vec+view_vec);
                    spec_amount = pow(std::max(0.0, dot(normal_vec, half_vec)), shininess);
                    //std::cerr << "dot product is " << dot(normal_vec, half_vec) << '\n';
                    //std::cerr << "specular: " << spec_amount << '\n';
                }
                else {
                    spec_amount = 0.0;
                }
                color col_specular = k_specular*spec_amount*col_light;

                // compute vertex color
                col = col_diffuse + col_specular;
                return true;
            }
        }

    public:
        color col_light;
        vec3 dir;
};

#endif
