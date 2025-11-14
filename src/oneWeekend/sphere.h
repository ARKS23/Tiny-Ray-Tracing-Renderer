#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable { // 实现hittable接口
public:
    sphere(const point3& center, double radius, shared_ptr<material> mat): center(center), radius(std::fmax(0, radius)) 
    , mat(mat) {}

public:
    virtual bool hit(const ray& r, interval ray_t, hit_record &record) const override {
        vec3 oc = center - r.origin();
        double a = r.direction().length_squared(); // 与自身dot等价
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - pow(radius, 2);
        double discriminant = h * h - a * c;
        if (discriminant < 0) return false; // 与球相离
        // 求根计算，目的是求最近的交点,并且需要在【t_min, t_max】有效范围内
        double sqrtd = std::sqrt(discriminant);
        double root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        /* 记录相交顶点的信息：对应的t，位置，法向量，该光线是否外部 */
        record.t = root;
        record.p = r.at(record.t);
        vec3 outward_normal = (record.p - center) / radius; // 单位化法向量
        record.set_front_face(r, outward_normal);
        record.mat = mat; // 记录材质

        return true;
    }

private:
    point3 center;  // 球心坐标
    double radius;  // 半径
    shared_ptr<material> mat; // 材质
};

#endif