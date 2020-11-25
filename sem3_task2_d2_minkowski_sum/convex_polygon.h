#pragma once

#include <iostream>
#include <vector>
#include "point_int2d.h"

class ConvexPolygon {
    std::vector<Point> points;

    void Resize(int n) {
        points.resize(n);
    }

    [[nodiscard]] size_t GetLeftVertexIndex() const;

    void MoveBegin(size_t new_first);

public:
    ConvexPolygon() = default;

    friend std::istream &operator>>(std::istream &in, ConvexPolygon &polygon);

    [[nodiscard]] long double GetArea() const;

    [[nodiscard]] ConvexPolygon GetMinkowskiSum(const ConvexPolygon &other) const;
};

std::istream &operator>>(std::istream &in, ConvexPolygon &polygon) {
    int size;
    in >> size;
    polygon.Resize(size);
    for (auto &point : polygon.points) {
        in >> point;
    }
    return in;
}

ConvexPolygon ConvexPolygon::GetMinkowskiSum(const ConvexPolygon &other) const {
    ConvexPolygon sum_polygon;
    ConvexPolygon this_ex = *this;
    ConvexPolygon other_ex = other;
    this_ex.MoveBegin(this_ex.GetLeftVertexIndex());
    other_ex.MoveBegin(other_ex.GetLeftVertexIndex());
    this_ex.points.emplace_back(this_ex.points[0]);
    this_ex.points.emplace_back(this_ex.points[1]);
    other_ex.points.emplace_back(other_ex.points[0]);
    other_ex.points.emplace_back(other_ex.points[1]);

    size_t i = 0, j = 0;
    while (i < this_ex.points.size() - 2 || j < other_ex.points.size() - 2) {
        sum_polygon.points.emplace_back(this_ex.points[i] + other_ex.points[j]);
        auto or_area = (this_ex.points[i + 1] - this_ex.points[i]).GetOrientedAreaWith(
                other_ex.points[j + 1] - other_ex.points[j]);
        if (or_area >= 0) {
            ++i;
        }
        if (or_area <= 0) {
            ++j;
        }
    }
    return sum_polygon;
}

void ConvexPolygon::MoveBegin(size_t new_first) {
    auto points_copy = points;
    for (int i = 0; i < points.size(); ++i) {
        points[i] = points_copy[(i + new_first) % points_copy.size()];
    }
}

long double ConvexPolygon::GetArea() const {
    long double area = 0;
    Point base = points[0];
    for (int i = 2; i < points.size(); ++i) {
        area += static_cast<long double>(std::abs((points[i] - base).GetOrientedAreaWith(points[i - 1] - base))) /
                2;
    }
    return area;
}

size_t ConvexPolygon::GetLeftVertexIndex() const {
    int idx = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i] < points[idx]) {
            idx = i;
        }
    }
    return idx;
}
