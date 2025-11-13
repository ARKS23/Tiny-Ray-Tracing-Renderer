#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

/* 局部注入命名空间，仅在当前文件内有效 */
namespace {
    using std::make_shared;
    using std::shared_ptr;
    using std::vector;
}

/* 维护一个可被光线击中的物体集合 */
class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) {add(object);}

public:
    void add(shared_ptr<hittable> object) {objects.push_back(object);}
    
    void clear() {objects.clear();}

    bool hit(const ray& r, double ray_t_min, double ray_t_max, hit_record &record) const override {
        hit_record temp_record;
        bool hit_anything = false;
        double closest_so_far = ray_t_max; // 只求最近的击中交点，否则颜色会被之后的物体颜色覆盖

        for (const shared_ptr<hittable>& object : objects) {
            if (object->hit(r, ray_t_min, closest_so_far, temp_record)) { // 这里上限设置当前的closest_so_far
                hit_anything = true;
                closest_so_far = temp_record.t; // 这里更新closest_so_far,即取最近的
                record = temp_record;
            }
        }

        return hit_anything;
    }

public:
    vector<shared_ptr<hittable>> objects;
};

#endif