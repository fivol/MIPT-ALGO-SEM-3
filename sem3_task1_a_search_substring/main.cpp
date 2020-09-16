//A. Поиск подстроки
//Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
//p ≤ 30000, n ≤ 300000.
//
//Использовать один из методов:
//Вариант 1. С помощью префикс-функции;
//Вариант 2. С помощью z-функции.


#include <iostream>
#include "./solution.cpp"

int main() {
    string pattern, text;
    cin >> pattern >> text;

    vector<int> patternPositions;
    findPatternPositions(patternPositions, text, pattern);

    for (auto pos: patternPositions) {
        cout << pos << ' ';
    }
    return 0;
}
