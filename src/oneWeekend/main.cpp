#include <iostream>
#include "utils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

int main() {
    // 材质
    shared_ptr<material> material_ground = make_shared<lambertian>(color(0.8f, 0.8f, 0.f));
    shared_ptr<material> material_lambertian = make_shared<lambertian>(color(0.12f, 0.42f, 0.33f));
    shared_ptr<material> material_metal_sphere = make_shared<metal>(color(0.8f, 0.8f, 0.8f), 0.12f);

    // 世界可碰撞物体
    hittable_list world;
    world.add(make_shared<sphere>(point3(-1.1f, 0.f, -1.5f), 0.5, material_metal_sphere)); // 放置一个金属球体
    world.add(make_shared<sphere>(point3(1.1f, 0.f, -1.5f), 0.5, material_lambertian)); // 放置一个哑光球体
    world.add(make_shared<sphere>(point3(0, -100.5f, -1), 100, material_ground)); // 放置一个超大球体作为地面

    // 摄像机
    camera cam;
    cam.set_samples_per_pixel(10);
    cam.set_max_depth(4);
    cam.render(world);
}