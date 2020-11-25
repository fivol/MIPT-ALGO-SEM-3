#pragma once

#include <cmath>

class Vector {
protected:
    double x;
    double y;

public:
    Vector(double x, double y) : x(x), y(y) {}

    Vector() = default;


    Vector operator-(const Vector &other) const {
        return Vector(x - other.x, y - other.y);
    }


    [[nodiscard]] double GetOrientedAreaWith(const Vector &other) const {
        return x * other.y - y * other.x;
    }

    [[nodiscard]] bool IsClockwiseWith(const Vector &other) const {
        return GetOrientedAreaWith(other) > 0;
    }

    [[nodiscard]] bool IsCounterClockwiseWith(const Vector &other) const {
        return GetOrientedAreaWith(other) < 0;
    }

    [[nodiscard]] double GetLength() const {
        return std::sqrt(x * x + y * y);
    }
};
