#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"

/* 该结构记录相交顶点的位置，法向量，和对应的t值，光线是否从外部射入 */
class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

public:
    void set_front_face(const ray& r, const vec3& out_normal) {
        /* 设置光线是外部射入还是内部射出，保持法线永远朝外，shading计算更稳定 */
        this->front_face = dot(r.direction(), out_normal) < 0.f;
        this->normal = front_face ? out_normal : -out_normal;
    }
};

/*
 * 抽象类包含一个hit函数，该函数接受一条光线作为参数，并添加一个有效区间
 * 有效命中区间 tmin < t < tmax 
 */
class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record &record) const = 0;
};

#endif