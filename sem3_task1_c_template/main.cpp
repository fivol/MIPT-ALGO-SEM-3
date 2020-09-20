#include <iostream>
#include "./StringTemplate.cpp"

int main() {
    std::string templateString, text;
    std::cin >> templateString >> text;

    StringTemplate parser;
    parser.compileTemplate(templateString);

    parser.parseText(text);

    return 0;
}
