#ifndef TORUS_H
#define TORUS_H
//==============================================================================================
// Author: Guanxiong Chen
//==============================================================================================

#include "rtweekend.h"

#include "hittable.h"


class torus : public hittable {
    public:
        torus() {}

        torus(point3 cen, vec3 nor, double rad1, double rad2, shared_ptr<material> m) {
            center = cen;
            normal = unit_vector(nor);
            r1 = rad1;
            r2 = rad2;
            mat_ptr = m;
        }
        
        /* If given ray can hit the torus, marches the ray until ray-torus distance
         * is less than dist_epsilon, and set the hit record. Returns true.
         * If not, returns false.
         */
        bool ray_march(const ray& r, double t_min, double t_max, double t_epsilon, int depth, hit_record& rec) const;

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        vec3 normal; // unitary
        double r1;
        double r2;
        shared_ptr<material> mat_ptr;
};

bool torus::ray_march(const ray& r, double t_min, double t_max, double t_epsilon, int depth, hit_record& rec) const {
    // computes distance
    double k = dot((r.origin()-center), normal);
    point3 p = r.origin()-k*normal;
    vec3 M = unit_vector(p - center);
    point3 m = center + r1 * M;
    double dist = (m - r.origin()).length() - r2;
    double t = dist / r.direction().length();

    // base case: depth==0 or t<=t_epsilon
    if (depth==0 || t<=t_epsilon) {
        if (depth==0 and t>t_epsilon) {
            return false;
        }
        else {
            if (t<t_min || t>t_max) {
                return false;
            }
            else {
                point3 pos_intersect = r.at(t);
                rec.p = pos_intersect;
                rec.t = t;
                double k1 = dot((pos_intersect-center), normal);
                point3 p1 = pos_intersect-k1*normal;
                vec3 M1 = unit_vector(p1 - center);
                point3 m1 = center + r1 * M1;
                vec3 surf_normal = pos_intersect - m1;
                rec.set_face_normal(r, surf_normal);
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    // recursive case: depth>0 and dist>dist_epsilon
    else {
        ray ray_new(r.at(t), r.direction());
        return torus::ray_march(ray_new, 1e-4, t_max, t_epsilon, depth-1, rec);
    }
}


bool torus::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // recursive ray marching
    return ray_march(r, 0.01, t_max, 0.02, 10, rec);
}


#endif
