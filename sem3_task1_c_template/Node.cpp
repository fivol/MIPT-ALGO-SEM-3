#include <iostream>
#include <memory>
#include <map>


struct Node {
    std::weak_ptr<Node> parent;
    std::map<char, std::shared_ptr<Node>> children;
    std::weak_ptr<Node> suffixLink;
    std::weak_ptr<Node> shortLink;
    size_t patternIndex = 0;
    bool terminated = false;
    char symbol = '@';
};
