#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iomanip>


//https://contest.yandex.ru/contest/20642/problems/

//Арсений уснул. И приснилась ему планета Ка-Пэкс, на которой протекают две реки.
// Эти реки достаточно необычны для землян: они текут строго от одной точки до другой точки напрямую в пространстве
// (так как в сне Арсения планета вовсе не круглая). Арсений решил, что он хочет прорыть тоннель между реками.
// Так как он ленивый,
// то и копать много он не хочет. Помогите Арсению найти, сколько-таки ему придется прорыть.


// For increase precision in sqrt calculation
const int precision_upper = 10000;

class Vector {
public:
    double x = 0;
    double y = 0;
    double z = 0;

    Vector() = default;

    ~Vector() = default;

    Vector(const Vector &other) = default;

    Vector(Vector &&other) = default;

    virtual Vector &operator=(const Vector &other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    [[nodiscard]] double GetLength() const {
        return sqrt((x * x + y * y + z * z) * precision_upper * precision_upper) / precision_upper;
    }

    void Normalize() {
        *this /= GetLength();
    }

    [[nodiscard]] Vector Normalized() const {
        Vector result = *this;
        result.Normalize();
        return result;
    }

    Vector operator-() const {
        return (Vector() - *this);
    }

    Vector &operator*=(double value) {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    Vector &operator/=(double value) {
        return *this *= 1 / value;
    }

    Vector &operator+=(const Vector &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector &operator-=(const Vector &other) {
        *this += -other;
        return *this;
    }

    Vector operator+(const Vector &other) const {
        Vector result = *this;
        result += other;
        return result;
    }

    Vector operator-(const Vector &other) const {
        Vector result = *this;
        result -= other;
        return result;
    }

    Vector operator*(const Vector &other) const {
        // Векторное произведение
        Vector result = *this;
        result.x = Matrix2x2Det(y, z, other.y, other.z);
        result.y = -Matrix2x2Det(x, z, other.x, other.z);
        result.z = Matrix2x2Det(x, y, other.x, other.y);
        return result;
    }

    [[nodiscard]] double GetScalarProduct(const Vector &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    [[nodiscard]] double GetCosWith(const Vector &other) const {
        // return cos this^other = this*other / |this|*|other|
        return GetScalarProduct(other) / (GetLength() * other.GetLength());
    }

    static double CountVolume(const Vector &v1, const Vector &v2, const Vector &v3) {
        return Matrix3x3Det(v1, v2, v3);
    }

    static double CountSquare(const Vector &v1, const Vector &v2) {
        return (v1 * v2).GetLength();
    }

    [[nodiscard]] bool IsSharpAngle(const Vector &other) const {
        return GetScalarProduct(other) > 0;
    }

private:
    static double Matrix2x2Det(double x11, double x12, double x21, double x22) {
        return x11 * x22 - x12 * x21;
    }

    static double Matrix3x3Det(const Vector &v1, const Vector &v2, const Vector &v3) {
        return v1.x * Matrix2x2Det(v2.y, v2.z, v3.y, v3.z) -
               v1.y * Matrix2x2Det(v2.x, v2.z, v3.x, v3.z) +
               v1.z * Matrix2x2Det(v2.x, v2.y, v3.x, v3.y);
    }
};

struct Point : Vector {

    friend std::istream &operator>>(std::istream &in, Point &point) {
        in >> point.x >> point.y >> point.z;
        return in;
    }

    [[nodiscard]] double GetDistance(const Point &other) const {
        return (*this - other).GetLength();
    }
};

class Segment {
public:
    Point p1;
    Point p2;

    Segment() = default;

    Segment(Point &p1, Point &p2) : p1(p1), p2(p2) {}

    friend std::istream &operator>>(std::istream &in, Segment &line) {
        in >> line.p1 >> line.p2;
        return in;
    }

    [[nodiscard]] bool IsIntersectWith(const Segment &other) const {
        // Assuming this and other lies in the same plane
        // Circle is p1 -> other.p1 -> p2 -> other.p2 ->
        Vector angle1 = (other.p1 - p1) * (p2 - other.p1);
        Vector angle2 = (p2 - other.p1) * (other.p2 - p2);
        Vector angle3 = (other.p2 - p2) * (p1 - other.p2);
        Vector angle4 = (p1 - other.p2) * (other.p1 - p1);

        return angle1.IsSharpAngle(angle2) &&
               angle1.IsSharpAngle(angle3) &&
               angle1.IsSharpAngle(angle4);
    }

    [[nodiscard]] Vector GetNormalBetweenLines(const Segment &other) const {
        Vector line = p2 - p1;
        Vector other_line = other.p2 - other.p1;

        Vector normal;

        double volume = Vector::CountVolume(line, other_line, other.p1 - p1);
        double square = Vector::CountSquare(line, other_line);
        if (square == 0) {
            return normal;
        }
        double height = volume / square;
        normal = line * other_line;
        normal.Normalize();
        normal *= height;
        return normal;
    }

    [[nodiscard]] double GetDistance(const Segment &other) const {
        double min_ends = std::min(std::min(GetDistance(other.p1), GetDistance(other.p2)),
                                   std::min(other.GetDistance(p1), other.GetDistance(p2)));

        Vector normal = GetNormalBetweenLines(other);
        Segment other_moved = other;
        other_moved.p1 -= normal;
        other_moved.p2 -= normal;

        // Now both Segments: this and other_moved lies in the same plane

        if (IsIntersectWith(other_moved)) {
            return normal.GetLength();
        }
        return min_ends;
    }

    [[nodiscard]] Vector GetDirectionVector() const {
        Vector result = p2 - p1;
        return result;
    }

    [[nodiscard]] double GetDistance(const Point &point) const {
        Vector dir = GetDirectionVector();
        if (dir.GetCosWith(point - p1) > 0 && (p1 - p2).GetCosWith(point - p2) > 0) {
            return Vector::CountSquare(dir, point - p1) / dir.GetLength();
        }
        return std::min(point.GetDistance(p1), point.GetDistance(p2));
    }

};


const size_t precision = 10;

int main() {
    Segment river1, river2;
    std::cin >> river1 >> river2;

    std::cout << std::fixed << std::setprecision(precision) << river1.GetDistance(river2);

    return 0;
}
