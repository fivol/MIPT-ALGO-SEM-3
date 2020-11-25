#include <iostream>
#include "StringTemplate.h"

int main() {
    std::string templateString, text;
    std::cin >> templateString >> text;

    StringTemplate parser(templateString);

    auto templateMatches = parser.parseText(text);

    for (auto match: templateMatches) {
        std::cout << match << ' ';
    }

    return 0;
}
