#include <iostream>
#include <vector>
#include <iomanip>
#include "convex_hull.h"

const int answer_precision = 10;


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

    std::cout << std::fixed << std::setprecision(answer_precision) << length;
    return 0;
}
