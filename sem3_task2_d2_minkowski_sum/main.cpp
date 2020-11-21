#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>


class Point;

class Vector {
public:
    int x;
    int y;

    Vector(int x, int y) : x(x), y(y) {}


    Vector() = default;

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
};


class Point : public Vector {
public:
    friend std::istream &operator>>(std::istream &in, Point &point) {
        in >> point.x >> point.y;
        return in;
    }

    Point(int x, int y) : Vector(x, y) {}

    explicit Point(Vector &&vec) : Vector(vec.x, vec.y) {}

    Point() = default;

    ~Point() = default;

    [[nodiscard]] Point Transposed() const {
        Point result = *this;
        std::swap(result.x, result.y);
        return result;
    }

    bool operator<(const Point &other) const {
        return std::make_pair(x, y) < std::make_pair(other.x, other.y);
    }

private:
};


class ConvexPolygon {
    std::vector<Point> points;

    void Resize(int n) {
        points.resize(n);
    }

    [[nodiscard]] size_t GetLeftVertexIndex() const {
        int idx = 0;
        for (int i = 1; i < points.size(); ++i) {
            if (points[i] < points[idx]) {
                idx = i;
            }
        }
        return idx;
    }

public:
    ConvexPolygon() {}

    ConvexPolygon(int n) : points(n) {}

    [[nodiscard]] size_t Size() const {
        return points.size();
    }

    friend std::istream &operator>>(std::istream &in, ConvexPolygon &polygon) {
        int size;
        in >> size;
        polygon.Resize(size);
        for (auto &point : polygon.points) {
            in >> point;
        }
        return in;
    }

    [[nodiscard]] long double GetArea() const {
        long double area = 0;
        Point base = points[0];
        for (int i = 2; i < points.size(); ++i) {
            area += static_cast<long double>(std::abs((points[i] - base).GetOrientedAreaWith(points[i - 1] - base))) /
                    2;
        }
        return area;
    }

    void MoveBegin(size_t new_first) {
        auto points_copy = points;
        for (int i = 0; i < points.size(); ++i) {
            points[i] = points_copy[(i + new_first) % points_copy.size()];
        }
    }

    [[nodiscard]] ConvexPolygon GetMinkowskiSum(const ConvexPolygon &other) const {
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
};


int main() {
    ConvexPolygon A, B;
    std::cin >> A >> B;

    std::cout << std::fixed << std::setprecision(6) << (A.GetMinkowskiSum(B).GetArea() - A.GetArea() - B.GetArea()) / 2;

    return 0;
}
