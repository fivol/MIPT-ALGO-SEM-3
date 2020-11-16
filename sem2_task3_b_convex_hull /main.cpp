#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>


class Vector {
public:
    double x;
    double y;

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

private:
};

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

private:
};


class ConvexHull {
public:

    explicit ConvexHull(std::vector<Point> &points) : points(points) {}

    void Build() {
        convex_hull.clear();
        if (points.size() <= 1) {
            return;
        }
        std::sort(points.begin(), points.end());

        Point left = points[0];
        Point right = points.back();
        std::vector<Point> top, bottom;
        top.emplace_back(left);
        bottom.emplace_back(left);

        for (size_t i = 1; i < points.size(); ++i) {
            if (i == points.size() - 1 || Point::IsLeftAngle(left, right, points[i])) {
                while (top.size() >= 2 && !Point::IsRightAngle(top[top.size() - 2], top.back(), points[i])) {
                    top.pop_back();
                }
                top.emplace_back(points[i]);
            }
            if (i == points.size() - 1 || Point::IsRightAngle(left, right, points[i])) {
                while (bottom.size() >= 2 && !Point::IsLeftAngle(bottom[bottom.size() - 2], bottom.back(), points[i])) {
                    bottom.pop_back();
                }
                bottom.emplace_back(points[i]);
            }
        }

        convex_hull.insert(convex_hull.begin(),
                           std::make_move_iterator(top.begin()),
                           std::make_move_iterator(top.end()));
        for (size_t i = bottom.size() - 2; i > 0; --i) {
            convex_hull.emplace_back(bottom[i]);
        }
    }

    double GetLength() {
        if (convex_hull.size() <= 1) {
            return 0;
        }
        double length = 0;
        for (size_t i = 1; i < convex_hull.size(); ++i) {
            length += (convex_hull[i] - convex_hull[i - 1]).GetLength();
        }
        length += (convex_hull[0] - convex_hull.back()).GetLength();


        return length;
    }

private:
    std::vector<Point> points;
    std::vector<Point> convex_hull;
};


int main() {
    int n;
    std::cin >> n;

    std::vector<Point> points(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> points[i];
    }

    ConvexHull hull(points);
    hull.Build();
    double length = hull.GetLength();

    std::cout << std::fixed << std::setprecision(10) << length;

    return 0;
}
