#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include "utils.h"
#include "interval.h"

using color = vec3;

void write_color(std::ostream &out, const color& pixel_color) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // [0, 1]范围映射回RGB[0, 255]
    static const interval intensity(0.000, 0.999);
    int rbyte = static_cast<int>(256 * intensity.clamp(r));
    int gbyte = static_cast<int>(256 * intensity.clamp(g));
    int bbyte = static_cast<int>(256 * intensity.clamp(b));

    // 写到输出流
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif