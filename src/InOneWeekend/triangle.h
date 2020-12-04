#ifndef TRIANGLE_H
#define TRIANGLE_H
//==============================================================================================
// Author: Guanxiong Chen
//==============================================================================================

#include "rtweekend.h"

#include "hittable.h"


class triangle : public hittable {
    public:
        triangle() {}

        triangle(point3 pt_a, point3 pt_b, point3 pt_c, shared_ptr<material> m)
            : pt_a(pt_a), pt_b(pt_b), pt_c(pt_c), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 pt_a;
        point3 pt_b;
        point3 pt_c;
        shared_ptr<material> mat_ptr;
        bool hit_plane(
            const ray& r, double t_min, double t_max, hit_record& rec) const;
        bool solve_bar_coords(
            const ray& r, hit_record& rec) const;
};

/* returns true if the given ray hits the plane on which the triangle lies;
 * sets the hit record
 * modified from my Matlab script for exams
 */
bool triangle::hit_plane(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 ray = unit_vector(r.direction());  // unitary ray vector
    point3 pos_camera = r.origin();
    vec3 perp = cross((pt_b - pt_a), (pt_c - pt_a));
    vec3 outward_normal = unit_vector(perp);
    point3 pos_p = pt_a;

    // get a
    vec3 w = pos_p - pos_camera;
    double a = dot(w, outward_normal);
    // get b
    double b = dot(ray, outward_normal);
    // get k
    if (check_equal(b, 0.0, 0.01)) {
        // ray is parallel to the plane
        return false;
    }
    double k = a/b;
    // convert k to t
    double t = k/r.dir.length();
    if (t<t_min || t>t_max) {
        // t outside of boundary
        return false;
    }
    point3 pos_intersect = pos_camera + k*ray;

    // set the hit record
    rec.t = t;
    rec.p = pos_intersect;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

/* solves for the barycentric coordinates of the hit point;
 * returns false if the sum of the coordinates > 1.
 * modified from my Matlab script for exams
 */
bool triangle::solve_bar_coords(const ray& r, hit_record& rec) const {
    point3 pt_p = rec.p;
    vec3 ab = pt_b-pt_a;
    vec3 ac = pt_c-pt_a;
    vec3 pb = pt_b-pt_p;
    vec3 pa = pt_a-pt_p;
    vec3 pc = pt_c-pt_p;
    
    double At = 0.5*cross(ab, ac).length();
    double Aa = 0.5*cross(pb, pc).length();
    double Ab = 0.5*cross(pa, pc).length();
    double Ac = 0.5*cross(pa, pb).length();
    
    double alpha = Aa/At;
    double beta = Ab/At;
    double gamma = Ac/At;
    if (!check_equal((alpha+beta+gamma), 1.0, 0.01)) {
        // point is outside the triangle
        return false;
    }
    else {
        return true;
    }
}

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // compute where the ray hits on the triangle's plane
    if (!hit_plane(r, t_min, t_max, rec)) {
        return false;
    }
    
    // check if the hit point is inside the triangle; returns false if not
    if (!solve_bar_coords(r, rec)) {
        return false;
    }
    
    return true;
}


#endif
