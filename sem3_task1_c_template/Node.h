#pragma once

#include <iostream>
#include <memory>
#include <map>


struct Node {
    Node(char symbol, std::shared_ptr<Node> &parent) : symbol(symbol), parent(parent) {};

    Node(std::shared_ptr<Node> &parent, std::shared_ptr<Node> &suffixLink) : parent(parent),
                                                                            suffixLink(suffixLink) {};

    std::weak_ptr<Node> parent;
    std::map<char, std::shared_ptr<Node>> children;
    std::weak_ptr<Node> suffixLink;
    std::weak_ptr<Node> shortLink;
    size_t patternIndex = 0;
    bool terminated = false;
    char symbol;
};
