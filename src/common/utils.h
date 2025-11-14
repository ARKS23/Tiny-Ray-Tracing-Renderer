#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

// 注入命名空间内容
using std::make_shared;
using std::shared_ptr;

// 一些常量
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// 工具函数
inline double degress_to_radians(double degress) {
    /* 角度转弧度 */
    return degress * pi / 180.f;
}

inline double random_double() {
    /* [0, 1)区间随机 */
    return std::rand() / (RAND_MAX + 1.f);
}

inline double random_double(double min_num, double max_num) {
    /* [min_num, max_num)区间随机 */
    return min_num + (max_num - min_num) * random_double();
}

// 通用头文件
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif