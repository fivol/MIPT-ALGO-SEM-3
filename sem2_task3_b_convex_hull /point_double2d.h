#pragma once

#include <iostream>
#include "vector_double2d.h"


class Point : public Vector {
public:
    friend std::istream &operator>>(std::istream &in, Point &point) {
        in >> point.x >> point.y;
        return in;
    }

    bool operator<(const Point &other) const {
        return x < other.x || x == other.x && y < other.y;
    }

    static bool IsRightAngle(const Point &base, const Point &p1, const Point &p2) {
        return (p1 - base).IsClockwiseWith(p2 - base);
    }

    static bool IsLeftAngle(const Point &base, const Point &p1, const Point &p2) {
        return (p1 - base).IsCounterClockwiseWith(p2 - base);
    }
};
