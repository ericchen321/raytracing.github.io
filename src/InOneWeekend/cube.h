#ifndef CUBE_H
#define CUBE_H
//==============================================================================================
// Author: Guanxiong Chen
//==============================================================================================

#include "rtweekend.h"

#include "hittable.h"
#include "triangle.h"


class cube : public hittable {
    public:
        cube() {}

        /* angle_x, angle_y, angle_z are angles in deg of rotation
         * wrt. each axis
         */
        cube(point3 cen, double w, double h, double d, double angle_x, double angle_y, double angle_z, shared_ptr<material> m) {
            // assign material
            mat_ptr = m;

            double half_w = w/2.0;
            double half_h = h/2.0;
            double half_d = d/2.0;
            // compute the cube's vertices
            point3 pt_a_local = rotate(point3(-1.0*half_w, half_h, -1.0*half_d), angle_x, angle_y, angle_z);
            pt_a = pt_a_local + cen;
            point3 pt_b_local = rotate(point3(-1.0*half_w, half_h, half_d), angle_x, angle_y, angle_z);
            pt_b = pt_b_local + cen;
            point3 pt_c_local = rotate(point3(half_w, half_h, half_d), angle_x, angle_y, angle_z);
            pt_c = pt_c_local + cen;
            point3 pt_d_local = rotate(point3(half_w, half_h, -1.0*half_d), angle_x, angle_y, angle_z);
            pt_d = pt_d_local + cen;
            point3 pt_e_local = rotate(point3(-1.0*half_w, -1.0*half_h, -1.0*half_d), angle_x, angle_y, angle_z);
            pt_e = pt_e_local + cen;
            point3 pt_f_local = rotate(point3(-1.0*half_w, -1.0*half_h, half_d), angle_x, angle_y, angle_z);
            pt_f = pt_f_local + cen;
            point3 pt_g_local = rotate(point3(half_w, -1.0*half_h, half_d), angle_x, angle_y, angle_z);
            pt_g = pt_g_local + cen;
            point3 pt_h_local = rotate(point3(half_w, -1.0*half_h, -1.0*half_d), angle_x, angle_y, angle_z);
            pt_h = pt_h_local + cen;

            // compute the cube's triangles
            tri_abd = triangle(pt_a, pt_b, pt_d, m);
            tri_bcd = triangle(pt_b, pt_c, pt_d, m);
            tri_efh = triangle(pt_e, pt_h, pt_f, m);
            tri_fgh = triangle(pt_f, pt_h, pt_g, m);
            tri_abe = triangle(pt_a, pt_e, pt_b, m);
            tri_bfe = triangle(pt_b, pt_e, pt_f, m);
            tri_cdh = triangle(pt_c, pt_h, pt_d, m);
            tri_cgh = triangle(pt_c, pt_g, pt_h, m);
            tri_bcg = triangle(pt_b, pt_g, pt_c, m);
            tri_bfg = triangle(pt_b, pt_f, pt_g, m);
            tri_ade = triangle(pt_a, pt_d, pt_e, m);
            tri_deh = triangle(pt_d, pt_e, pt_h, m);
            for (int i=0; i<12; i++) {
                switch(i) {
                    case 0:
                        triangles[i] = &tri_abd;
                        break;
                    case 1:
                        triangles[i] = &tri_bcd;
                        break;
                    case 2:
                        triangles[i] = &tri_efh;
                        break;
                    case 3:
                        triangles[i] = &tri_fgh;
                        break;
                    case 4:
                        triangles[i] = &tri_abe;
                        break;
                    case 5:
                        triangles[i] = &tri_bfe;
                        break;
                    case 6:
                        triangles[i] = &tri_cdh;
                        break;
                    case 7:
                        triangles[i] = &tri_cgh;
                        break;
                    case 8:
                        triangles[i] = &tri_bcg;
                        break;
                    case 9:
                        triangles[i] = &tri_bfg;
                        break;
                    case 10:
                        triangles[i] = &tri_ade;
                        break;
                    case 11:
                        triangles[i] = &tri_deh;
                        break;
                    default:
                        break;
                    }
            }
        };

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 pt_a;
        point3 pt_b;
        point3 pt_c;
        point3 pt_d;
        point3 pt_e;
        point3 pt_f;
        point3 pt_g;
        point3 pt_h;
        triangle tri_abd;
        triangle tri_bcd;
        triangle tri_efh;
        triangle tri_fgh;
        triangle tri_abe;
        triangle tri_bfe;
        triangle tri_cdh;
        triangle tri_cgh;
        triangle tri_bcg;
        triangle tri_bfg;
        triangle tri_ade;
        triangle tri_deh;
        triangle* triangles[12];
        shared_ptr<material> mat_ptr;
};


bool cube::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    bool if_hit = false;
    rec.t = t_max;
    
    for (int i=0; i<12; i++) {
        hit_record rec_i;
        bool if_hit_i = triangles[i]->hit(r, t_min, t_max, rec_i);
        if (if_hit_i && rec_i.t<rec.t) {
            rec = rec_i;
            if_hit = if_hit_i;
        }
    }

    return if_hit;
}


#endif
