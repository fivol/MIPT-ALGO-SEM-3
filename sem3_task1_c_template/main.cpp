#include <iostream>
#include "StringTemplate.h"

int main() {
    std::string templateString, text;
    std::cin >> templateString >> text;

    StringTemplate parser(templateString);

    parser.parseText(text);

    return 0;
}
