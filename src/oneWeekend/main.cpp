#include <iostream>
#include "color.h"
#include "ray.h"

double hit_sphere_old(const point3 &center, double radius, const ray &r) {
    /* 该函数判断射线与球面的关系: 推导过程在笔记上 */
    vec3 oc = center - r.origin();
    double a = dot(r.direction(), r.direction());
    double b = -2.f * dot(r.direction(), oc);
    double c = dot(oc, oc) - pow(radius, 2);
    double discriminant = pow(b, 2) - 4.f * a * c;
    
    if (discriminant < 0) { // 无交点
        return -1.0;
    }
    else {
        return (-b - std::sqrt(discriminant)) / (2.0 * a);
    }
}

double hit_sphere(const point3 &center, double radius, const ray &r) {
    /* 优化版本，数值更加稳定。该函数判断射线与球面的关系: 推导过程在笔记上 */
    vec3 oc = center - r.origin();
    double a = r.direction().length_squared(); // 与自身dot等价
    double h = dot(r.direction(), oc);
    double c = oc.length_squared() - pow(radius, 2);
    double discriminant = h * h - a * c;
    
    if (discriminant < 0) { // 无交点
        return -1.0;
    }
    else {
        return (h - std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray &r) {
    double t = hit_sphere(point3(0, 0, -1), 0.5, r); // 计算与球面的关系
    if (t > 0.0) {
        vec3 normal = unit_vector(r.at(t) - vec3(0, 0, -1)); // 球面点对应的法线
        return 0.5 * color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0); // 参数a根据y的大小限制在[0, 1]区间
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // 线形插值做出渐变色
}

/* image preset */
const float aspect_ratio = 16.0 / 9.0;
const int image_width = 1920;
const int image_height = ( static_cast<int>(image_width / aspect_ratio) ) < 1 ? 1 : ( static_cast<int>(image_width / aspect_ratio) );

/* view_port */
const double viewport_height = 2.0f;
const double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

int main() {
    // 摄像机
    double focal_length =1.f;
    point3 camera_center = point3(0, 0, 0);

    // 沿适口水平边和垂直边的向量
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // 计算水平和垂直步长向量
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // 计算左上角的像素位置
    vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2; // z轴焦距变换 + 水平和垂直变换
    vec3 pixel_00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); // 计算左上角(0, 0)位置像素的中心点位置

    /* render */
    // 踩坑:这段代码要写上，不然图片预览器不知道是什么文件
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining:" << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 pixel_center = pixel_00_loc + (i * pixel_delta_u) + (j * pixel_delta_v); // 计算当前像素的世界坐标
            vec3 ray_direction = pixel_center - camera_center;  // 相机中心指向当前像素的方向
            ray r(camera_center, ray_direction); // 构造光线对象，方便后续计算

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.           \n";
}