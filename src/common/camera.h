#ifndef CAMERA_H
#define CAMERA_H

#include "hittable_list.h"
#include "utils.h"
#include "color.h"
#include "material.h"

class camera {
public:
    void render(const hittable_list& world) {
        initialize();

        /* render */
        std::ofstream outfile(output_file_path);
        // 踩坑:这段代码要写上，不然图片预览器不知道是什么文件
        outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining:" << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < sample_per_pixel; ++sample) { // 抗锯齿，像素点采样
                    ray r = get_ray(i, j); // 获得采样点对应的光线
                    pixel_color += ray_color(r, world, max_depth); // 计算光线的最近碰撞
                }
                write_color(outfile, pixel_color * pixel_samples_scale); // 混合颜色
            }
        }
        std::clog << "\rDone.           \n";
        outfile.close();
    }

public:
    /* setter */
    void set_samples_per_pixel(unsigned int num) {
        /* 设置采样率 */
        sample_per_pixel = num;
        set_pixel_samples_scale();
    }

    void set_max_depth(unsigned int depth) {
        max_depth = depth;
    }

private:
    void initialize() {
        /* 初始化变量 */
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0.f, 0.f, 0.f);

        // 视口设置
        double focal_length = 1.f; // 焦距
        double theta = degress_to_radians(vertical_fov);
        double h = std::tan(theta / 2);
        double viewport_height = 2.0 * h * focal_length;
        double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // 沿适口水平边和垂直边的向量
        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);

        // 计算水平和垂直步长向量
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // 计算左上角的像素位置
        vec3 viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2; // z轴焦距变换 + 水平和垂直变换
        pixel_00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); // 计算左上角(0, 0, -1)位置像素的中心点位置

        // 采样率权重设置
        set_pixel_samples_scale();
    }

    color ray_color(const ray& r, const hittable_list& world, int depth) const {
        if (depth <= 0) // 光线弹射到达最大次数，递归出口
            return color(0.f, 0.f, 0.f);

        /* 对世界中的物体进行光线碰撞，计算出像素对应的颜色 */
        hit_record record;
        if (world.hit(r, interval(0.001, infinity), record)) { // 由于浮点数误差，t_min为0的情况会让光线卡在表面弹射，直至消耗完递归深度，这类bug名为shadow ance
            ray scattered;
            color attenuation;
            if (record.mat->scatter(r, record, attenuation, scattered)) { // 物体反射光线
                return attenuation * ray_color(scattered, world, depth - 1); // 光线弹射，并进行光强衰减
            }
            else {  // 完全吸收的情况
                return color(0.f, 0.f, 0.f);
            }
        }

        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0); // 参数a根据y的大小限制在[0, 1]区间
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // 线形插值做出渐变色
    }

    ray get_ray(int i, int j) const {
        /* 像素点内随机采样，构造光线 */
        vec3 offset = pixel_sample_square(); // 随机偏移量
        vec3 pixel_sample = pixel_00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v); // 采样点

        vec3 ray_origin = center;
        vec3 ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    void set_pixel_samples_scale() {
        /* 设置采样权重 */
        pixel_samples_scale = 1.0 / sample_per_pixel;
    }

    vec3 pixel_sample_square() const {
        /* 计算偏移量 */
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

private:
    const std::string output_file_path = "/Users/ark/图形学/Code/RayTracingPratice/image/dielectric_Schlick_material.ppm";
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 1920;
    double vertical_fov = 45.0;
    int image_height;
    point3 center;
    point3 pixel_00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

private:
    unsigned int sample_per_pixel = 10; // 像素点采样率
    double pixel_samples_scale; // 像素点权重,最后取平均要用到
    unsigned int max_depth = 10; // 光线弹射次数限制
};

#endif