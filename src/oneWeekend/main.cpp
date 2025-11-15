#include <iostream>
#include "utils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

int main() {
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