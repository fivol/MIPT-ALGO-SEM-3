#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "point_double2d.h"



class ConvexHull {
    std::vector<Point> points_;
    std::vector<Point> convex_hull_;
public:

    explicit ConvexHull(std::vector<Point> &points) : points_(points) {}

    void Build();

    double GetLength();
};

void ConvexHull::Build() {
    convex_hull_.clear();
    if (points_.size() <= 1) {
        return;
    }
    std::sort(points_.begin(), points_.end());

    Point left = points_[0];
    Point right = points_.back();
    std::vector<Point> top, bottom;
    top.emplace_back(left);
    bottom.emplace_back(left);

    for (size_t i = 1; i < points_.size(); ++i) {
        if (i == points_.size() - 1 || Point::IsLeftAngle(left, right, points_[i])) {
            while (top.size() >= 2 && !Point::IsRightAngle(top[top.size() - 2], top.back(), points_[i])) {
                top.pop_back();
            }
            top.emplace_back(points_[i]);
        }
        if (i == points_.size() - 1 || Point::IsRightAngle(left, right, points_[i])) {
            while (bottom.size() >= 2 && !Point::IsLeftAngle(bottom[bottom.size() - 2], bottom.back(), points_[i])) {
                bottom.pop_back();
            }
            bottom.emplace_back(points_[i]);
        }
    }

    convex_hull_.insert(convex_hull_.begin(),
                        std::make_move_iterator(top.begin()),
                        std::make_move_iterator(top.end()));
    for (size_t i = bottom.size() - 2; i > 0; --i) {
        convex_hull_.emplace_back(bottom[i]);
    }
}

double ConvexHull::GetLength() {
    if (convex_hull_.size() <= 1) {
        return 0;
    }
    double length = 0;
    for (size_t i = 1; i < convex_hull_.size(); ++i) {
        length += (convex_hull_[i] - convex_hull_[i - 1]).GetLength();
    }
    length += (convex_hull_[0] - convex_hull_.back()).GetLength();


    return length;
}
