#include <iostream>
#include <iomanip>
#include "convex_polygon.h"


const int precision = 6;

long double calculateCoefficientY(ConvexPolygon &A, ConvexPolygon &B) {
    return (A.GetMinkowskiSum(B).GetArea() - A.GetArea() - B.GetArea()) / 2;
}


int main() {
    ConvexPolygon A, B;
    std::cin >> A >> B;

    std::cout << std::fixed << std::setprecision(precision) << calculateCoefficientY(A, B);
    return 0;
}

