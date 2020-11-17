#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>
#include <set>

const double eps = 1E-9;

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

    Vector &operator*=(int value) {
        x *= value;
        y *= value;
        return *this;
    }


    [[nodiscard]] int GetOrientedAreaWith(const Vector &other) const {
        return x * other.y - y * other.x;
    }

    [[nodiscard]] int GetOrientedAreaSign(const Vector &other) const {
        int area = GetOrientedAreaWith(other);
        if (!area) {
            return 0;
        }
        if (area > 0) {
            return 1;
        }
        return -1;
    }

    [[nodiscard]] int64_t GetScalarProduct(const Vector &other) const {
        return static_cast<int64_t>(x * other.x) + y * other.y;
    }

    [[nodiscard]] bool IsCoDirectionalWith(const Vector &other) const {
        return GetScalarProduct(other) > 0;
    }

    [[nodiscard]] double GetLength() const {
        return sqrt(x * x + y * y);
    }

    [[nodiscard]] bool IsZero() const {
        return !x && !y;
    }

private:
};


class Point : public Vector {
public:
    friend std::istream &operator>>(std::istream &in, Point &point) {
        in >> point.x >> point.y;
        return in;
    }

    Point(int x, int y) : Vector(x, y) {}

    Point() = default;

    ~Point() = default;

    [[nodiscard]] Point Transposed() const {
        Point result = *this;
        std::swap(result.x, result.y);
        return result;
    }

private:
};

class Segment {
public:
    Point p1;
    Point p2;

    Segment() = default;

    ~Segment() = default;

    Segment(Point &_p1, Point &_p2) : p1(_p1), p2(_p2) {
        if (p2.x < p1.x) {
            std::swap(p1, p2);
        }
        if (p1.x == p2.x && p2.y < p1.y) {
            std::swap(p1, p2);
        }
    }

    bool operator<(const Segment &other) const {
        double x = std::max(std::min(p1.x, p2.x), std::min(other.p1.x, other.p2.x));
        return GetY(x) < other.GetY(x) - eps;
    }

    [[nodiscard]] double GetY(double x) const {
        if (p1.x == p2.x) return p1.y;
        return (double) p1.y + (double) (p2.y - p1.y) * (x - p1.x) / (double) (p2.x - p1.x);
    }

    friend std::istream &operator>>(std::istream &in, Segment &line) {
        in >> line.p1 >> line.p2;
        return in;
    }

    [[nodiscard]] Vector GetDirectionVector() const {
        return p2 - p1;
    }

    static bool IsInCenter(int a, int b, int x) {
        return a <= x && x <= b || a >= x && x >= b;
    }

    static bool IsIntersect(int a1, int b1, int a2, int b2) {
        return IsInCenter(a1, b1, a2) || IsInCenter(a1, b1, b2) || IsInCenter(a2, b2, a1);
    }

    [[nodiscard]] bool isIntersectWith(const Segment &other) const {
        auto this_direction = GetDirectionVector();
        auto other_direction = other.GetDirectionVector();
        int other_p1 = this_direction.GetOrientedAreaSign(other.p1 - p1);
        int other_p2 = this_direction.GetOrientedAreaSign(other.p2 - p1);
        int this_p1 = other_direction.GetOrientedAreaSign(p1 - other.p1);
        int this_p2 = other_direction.GetOrientedAreaSign(p2 - other.p1);
        if (!other_p1 && !other_p2 && !this_p1 && !this_p2) {
            // 2 отрезка лежат на одной прямой
            return IsIntersect(p1.x, p2.x, other.p1.x, other.p2.x) && IsIntersect(p1.y, p2.y, other.p1.y, other.p2.y);
        }
        return !(other_p1 * other_p2 > 0 || this_p1 * this_p2 > 0);
    }
};

class ScanLine {
    std::vector<Segment> segments;

    struct Event {
        enum EventType {
            SEG_BEGIN,
            SEG_END,
        };
        int x;
        int id;
        EventType type;

        Event(EventType type, int x, int id) : type(type), x(x), id(id) {}

        ~Event() = default;


        bool operator<(const Event &other) const {
            if (x != other.x) return x < other.x;
            return type < other.type;
        }
    };

    struct Status {
        Segment seg;
        int id;

        bool operator<(const Status &other) const {
            return seg < other.seg;
        }

        Status(Segment seg, int id) : seg(seg), id(id) {}

        ~Status() = default;

        static std::set<Status>::iterator GetNext(std::set<Status>::iterator it) {
            return ++it;
        }

        static std::set<Status>::iterator GetPrev(std::set<Status>::iterator it, const std::set<Status> &set_) {
            return it == set_.begin() ? set_.end() : --it;
        }
    };

public:
    ScanLine(std::vector<Segment> &segments) : segments(segments) {}

    ScanLine() = default;

    ~ScanLine() = default;


    [[nodiscard]] std::pair<int, int> GetOverlappingSegmentIdx() const {
        std::vector<std::set<Status>::iterator> where(segments.size());
        std::vector<Event> raw_events;

        for (size_t i = 0; i < segments.size(); ++i) {
            auto seg = segments[i];
            raw_events.emplace_back(Event(Event::SEG_BEGIN, std::min(seg.p1.x, seg.p2.x), i));
            raw_events.emplace_back(Event(Event::SEG_END, std::max(seg.p1.x, seg.p2.x), i));
        }
        std::sort(raw_events.begin(), raw_events.end());
        std::set<Status> scan_status;

        for (const auto &event: raw_events) {
            int id = event.id;
            if (event.type == Event::SEG_BEGIN) {
                auto next = scan_status.lower_bound(Status(segments[id], id));
                auto prev = Status::GetPrev(next, scan_status);
                if (next != scan_status.end() && next->seg.isIntersectWith(segments[id])) {
                    return {next->id, id};
                }
                if (prev != scan_status.end() && prev->seg.isIntersectWith(segments[id])) {
                    return {prev->id, id};
                }
                where[id] = scan_status.insert(next, Status(segments[id], id));

            } else {
                auto next = Status::GetNext(where[id]);
                auto prev = Status::GetPrev(where[id], scan_status);
                if (next != scan_status.end() && prev != scan_status.end() && prev->seg.isIntersectWith(next->seg)) {
                    return {prev->id, next->id};
                }
                scan_status.erase(where[id]);
            }
        }
        return {-1, -1};
    }

};


int main() {
    int n;

    std::cin >> n;
    std::vector<Segment> segments(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> segments[i];
    }

    std::pair<int, int> overlapping_segment = ScanLine(segments).GetOverlappingSegmentIdx();

    if (overlapping_segment.first == -1) {
        std::cout << "NO\n";
    } else {
        std::cout << "YES\n";
        std::cout << overlapping_segment.first + 1 << ' ' << overlapping_segment.second + 1;
    }

    return 0;
}