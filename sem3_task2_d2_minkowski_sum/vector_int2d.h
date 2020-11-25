#pragma once

#include <cinttypes>


class Vector {
protected:
    int x;
    int y;
public:
    Vector() = default;

    Vector(int x, int y) : x(x), y(y) {}

    ~Vector() = default;

    Vector operator-(const Vector &other) const {
        return Vector(x - other.x, y - other.y);
    }

    Vector operator+(const Vector &other) const {
        return Vector(x + other.x, y + other.y);
    }

    Vector &operator*=(int value) {
        x *= value;
        y *= value;
        return *this;
    }

    [[nodiscard]] int64_t GetOrientedAreaWith(const Vector &other) const {
        return static_cast<int64_t>(x) * other.y - static_cast<int64_t>(y) * other.x;
    }

    [[nodiscard]] int getX() const {
        return x;
    }

    [[nodiscard]] int getY() const {
        return y;
    }
};

