#pragma once

#include <iostream>
#include <memory>
#include <map>


struct Node {
    Node(char symbol, std::shared_ptr<Node> &parent):symbol(symbol), parent(parent){};
    explicit Node(char symbol):symbol(symbol){};
    std::weak_ptr<Node> parent;
    std::map<char, std::shared_ptr<Node>> children;
    std::weak_ptr<Node> suffixLink;
    std::weak_ptr<Node> shortLink;
    size_t patternIndex = 0;
    bool terminated = false;
    char symbol = '@';
};
