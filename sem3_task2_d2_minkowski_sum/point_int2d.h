#pragma once

#include "vector_int2d.h"
#include <istream>

class Point : public Vector {
public:
    friend std::istream &operator>>(std::istream &in, Point &point);

    Point(int x, int y) : Vector(x, y) {}

    Point() = default;

    explicit Point(Vector &&vec) : Vector(vec.getX(), vec.getY()) {}

    ~Point() = default;

    bool operator<(const Point &other) const {
        return std::make_pair(x, y) < std::make_pair(other.x, other.y);
    }
};

std::istream &operator>>(std::istream &in, Point &point) {
    in >> point.x >> point.y;
    return in;
}
