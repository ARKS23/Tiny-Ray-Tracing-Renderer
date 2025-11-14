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

    bool hit(const ray& r, interval ray_t, hit_record &record) const override {
        /* 尝试对集合中的物体进行碰撞检测 */
        hit_record temp_record;
        bool hit_anything = false;
        double closest_so_far = ray_t.max_; // 只求最近的击中交点，否则颜色会被之后的物体颜色覆盖

        for (const shared_ptr<hittable>& object : objects) {
            if (object->hit(r, interval(ray_t.min_, closest_so_far), temp_record)) { // 这里上限设置当前的closest_so_far
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