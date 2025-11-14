#include <iostream>
#include "utils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main() {
    // 世界可碰撞物体
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5)); // 放置一个球体
    world.add(make_shared<sphere>(point3(0, -100.5f, -1), 100)); // 放置一个球体

    // 摄像机
    camera cam;
    cam.set_samples_per_pixel(20);
    cam.set_max_depth(4);
    cam.render(world);
}