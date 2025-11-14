#ifndef INTERVAL_H
#define INTERVAL_H
#include <limits>

/* 区间类工具类，用于管理最小值和最大值的实值区间 */
class interval {
public:
    interval() : min_(std::numeric_limits<double>::infinity()), max_(-std::numeric_limits<double>::infinity()) {} //默认空区间
    interval(double min, double max): min_(min), max_(max) {}

public:
    double size() const { return max_ - min_;}

    bool contains(double x) const {return min_ <= x && x <= max_;}

    bool surrounds(double x) const {return min_ < x && x < max_;}

    double clamp(double x) const {
        if (x < min_) return min_;
        if (x > max_) return max_;
        return x;
    }

public:
    double min_, max_;
    static const interval empty, universe;
};

// 初始化静态成员函数，防止链接报错
inline const interval interval::empty = interval(+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
inline const interval interval::universe = interval(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());

#endif