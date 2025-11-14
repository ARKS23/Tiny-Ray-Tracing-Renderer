#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"

/*
 * 抽象基类：1.产生散射光线 2.发生散射的情况下衰减多少.
 */
class material {
public:
    virtual ~material() = default;

    /* 参数：
      1. 入射光线
      2. hit表面点的记录
      3. 反射颜色
      4. 反射光线 
    */
    virtual bool scatter(const ray &r_in, const hit_record& record, color& attenuation, ray& scattered) const = 0;
};


/* 理想漫反射：哑光效果 */
class lambertian : public material {
public:
    lambertian(const color& albedo_) : albedo(albedo_) {}

public:
    virtual bool scatter(const ray &r_in, const hit_record& record, color& attenuation, ray& scattered) const override {
        vec3 scatter_direction =  record.normal + random_unit_vector(); // 漫反射方向
        if (scatter_direction.near_zero()) // 防止漫反射方向完全为0的情况出现后续计算灾难
            scatter_direction = record.normal;
        scattered = ray(record.p, scatter_direction); // 设置反射光线
        attenuation = albedo;
        return true; // 总是反射，不吸收光线
    }

private:
    color albedo; // 反照率，材质的基础颜色
};

/* 具有反射功能的金属材料 */
class metal: public material {
public:
    metal(const color& albedo_, double fuzz) : albedo(albedo_), fuzz(fuzz) {}

public:
    virtual bool scatter(const ray &r_in, const hit_record& record, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(r_in.direction(), record.normal); // 计算反射光线方向
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector()); //模糊反射
        scattered = ray(record.p, reflected); // 设置反射光线
        attenuation = albedo;
        return (dot(scattered.direction(), record.normal) > 0); //符合光线反射角度
    }

private:
    color albedo;
    double fuzz = 0.3; // 模糊反射
};

#endif