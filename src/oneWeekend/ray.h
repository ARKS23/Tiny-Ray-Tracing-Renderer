#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    /* 构造函数重载，待补充 */
    ray() {}
    ray(const point3& origin, const vec3& direction): origin_(origin), direction_(direction){}

public:
    /* getter */
    const point3& origin() const {return origin_;}
    const vec3& direction() const {return direction_;}

    point3 at(double t) const {
        // 位置计算
        return origin_ + direction_ * t;
    }

private:
    point3 origin_;
    vec3 direction_;
};

#endif