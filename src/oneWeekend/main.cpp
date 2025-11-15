#include <iostream>
#include "utils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

void scene1() {
    // 材质
    shared_ptr<material> material_ground = make_shared<lambertian>(color(0.8f, 0.8f, 0.f));
    shared_ptr<material> material_lambertian = make_shared<lambertian>(color(0.12f, 0.32f, 0.63f));
    shared_ptr<material> material_metal_sphere = make_shared<metal>(color(0.8f, 0.8f, 0.8f), 0.12f);
    shared_ptr<material> material_metal_reflect = make_shared<metal>(color(0.8f, 0.8f, 0.8f), 0.01f);
    shared_ptr<material> material_dielectric = make_shared<dielectric>(1.1f);
    shared_ptr<material> material_bubble_dielectric = make_shared<dielectric>(1.f / 1.5f);

    // 世界可碰撞物体
    hittable_list world;
    world.add(make_shared<sphere>(point3(-1.1f, 0.f, -1.5f), 0.35, material_metal_sphere)); // 放置一个金属球体
    world.add(make_shared<sphere>(point3(1.1f, 0.f, -1.5f), 0.45, material_lambertian)); // 放置一个哑光球体
    world.add(make_shared<sphere>(point3(0, -100.5f, -1), 100, material_ground)); // 放置一个超大球体作为地面
    world.add(make_shared<sphere>(point3(0.f, 0.35f, -1.5f), 0.35, material_metal_reflect)); // 放置一个折射球体
    world.add(make_shared<sphere>(point3(1.1f, -0.25f, -0.75f), 0.25, material_bubble_dielectric)); // 放置一个空心玻璃球体
    world.add(make_shared<sphere>(point3(-1.1f, 1.2f, -1.5f), 0.45, material_dielectric)); // 放置一个全反射金属球体

    // 摄像机
    camera cam;
    cam.set_samples_per_pixel(20);
    cam.set_max_depth(4);
    cam.render(world);
}

void scene_main() {
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

    camera cam;
    cam.set_max_depth(8);
    cam.set_samples_per_pixel(50);

    //cam.render(world);
    cam.render_multi_threads(world);
}

int main() {
    scene_main();
    return 0;
}