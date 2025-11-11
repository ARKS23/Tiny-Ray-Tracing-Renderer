#include <iostream>
#include "color.h"

int main() {
    /* image preset */
    int image_width = 800;
    int image_height = 800;

    /* render */
    // 踩坑，这段代码要写上，不然图片预览器不知道是什么文件
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_width; ++i) {
        std::clog << "\rScanlines remaining:" << (image_height - i) << ' ' << std::flush;
        for (int j = 0; j < image_height; ++j) {
            double r = double(i) / (image_width - 1);
            double g = double(j) / (image_height - 1);
            double b = 0.f;

            color pixel_color(r, g, b);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.           \n";
}