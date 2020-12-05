//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "cube.h"
#include "light.h"
#include "light_list.h"
#include "point_light.h"

#include <iostream>

struct world_and_lights
{
    hittable_list world;
    light_list lights;
};

color ray_color(const ray& r, const world_and_lights& world_and_lights, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world_and_lights.world.hit(r, 0.001, infinity, rec)) {
        color color_under_light = color(0,0,0);
        bool if_not_under_shadow = world_and_lights.lights.compute_color(world_and_lights.world, rec, color_under_light);
        if (if_not_under_shadow) {
            // if the hit point is not under shadow, keep tracing
            ray scattered;
            color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                // scatter ray is produced, keep tracing
                return attenuation * ray_color(scattered, world_and_lights, depth-1);
            }
            else {
                // no scatter ray, return color under light
                return color_under_light;
            }
        }
        else {
            // if under shadow, return black
            return color(0,0,0);
        }
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


struct world_and_lights random_scene_with_spheres() {
    struct world_and_lights world_and_lights;

    // create world
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // create lights
    light_list lights;
    lights.add(make_shared<point_light>(color::random(), point3(-20, 8, 3)));

    world_and_lights.world = world;
    world_and_lights.lights = lights;
    return world_and_lights;
}

struct world_and_lights scene_with_sphere() {
    struct world_and_lights world_and_lights;

    // create world
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    point3 center(0, 0.75, 1);
     // glass
    shared_ptr<material> sphere_material = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(center, 0.7, sphere_material));
    
    // create lights
    light_list lights;
    lights.add(make_shared<point_light>(color::random(), point3(-20, 8, 3)));

    world_and_lights.world = world;
    world_and_lights.lights = lights;
    return world_and_lights;
}

struct world_and_lights scene_with_cube() {
    struct world_and_lights world_and_lights;

    // create world
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    point3 center(0, 0.75, 1);
     // diffuse
    auto albedo = color::random() * color::random();
    shared_ptr<material> cube_material = make_shared<lambertian>(albedo);
    world.add(make_shared<cube>(center, 1.5, 1.5, 1.5, 0, 0, 0, cube_material));
    
    // create lights
    light_list lights;
    lights.add(make_shared<point_light>(color::random(), point3(-20, 8, 3)));

    world_and_lights.world = world;
    world_and_lights.lights = lights;
    return world_and_lights;
}

hittable_list random_scene_with_cubes() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -3; a < 5; a++) {
        for (int b = -1; b < 3; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
            double angle_x = random_double(-30, 30);
            double angle_y = random_double(-30, 30);
            double angle_z = random_double(-30, 30);

            if ((center - point3(4, 0.2, 0)).length() > 1.0) {
                //std::cerr << "center: " << center.e[0] << ' ' << center.e[1] << ' ' << center.e[2] << '\n';
                shared_ptr<material> cube_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    cube_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<cube>(center, 0.4, 0.4, 0.4, angle_x, angle_y, angle_z, cube_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    cube_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<cube>(center, 0.4, 0.4, 0.4, angle_x, angle_y, angle_z, cube_material));
                } else {
                    // glass
                    cube_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<cube>(center, 0.4, 0.4, 0.4, angle_x, angle_y, angle_z, cube_material));
                }
            }
        }
    }

    return world;
}


int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 12;

    // World

    auto world_and_lights = random_scene_with_spheres();
    //auto world_and_lights = scene_with_cube();
    //auto world_and_lights = scene_with_sphere();

    // Camera

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world_and_lights, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
