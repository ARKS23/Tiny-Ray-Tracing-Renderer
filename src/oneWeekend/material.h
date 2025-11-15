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
      1. 入射光线 (传入)
      2. hit表面点的记录 (传入)
      3. 反射颜色 (传出)
      4. 反射光线 (传出)
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

/* 会发生折射的玻璃材质 */
class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

public:
    bool scatter(const ray& r_in, const hit_record& record, color& attenuation, ray& scattered) const override {
        attenuation = color(1.f, 1.f, 1.f); // 这里让透明材质不吸收光能，只改变方向
        double ri = record.front_face ? (1.0 / refraction_index) : refraction_index; // 这里需要区分从外部到内部还是从内部到外部
        
        vec3 unit_direction = unit_vector(r_in.direction()); // 入射向量单位化
        double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.f);
        double sin_theta = std::sqrt(1.f - pow(cos_theta, 2));

        bool must_reflect = ri * sin_theta > 1.f;  // 这种情况下斯涅尔定律无解，需要反射光线
        vec3 direction;

        if (must_reflect || reflectance(cos_theta, ri) > random_double())
            direction = reflect(r_in.direction(), record.normal); // 发生反射
        else
            direction = refract(r_in.direction(), record.normal, ri); // 发生折射

        scattered = ray(record.p, direction); // 构造折射光线
        return true;
    }

private:
    static double reflectance(double cosine, double refraction_index) {
        /* 施利克近似: 入射角越大反射的概率越高 */
        double r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = pow(r0, 2);
        return r0 + (1 - r0) * pow(1 - cosine, 5);
    }

private:
    double refraction_index; // 折射率
};

#endif