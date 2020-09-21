#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "Node.h"
#include "StringTemplate.h"



void StringTemplate::buildTree() {
    root = std::make_shared<Node>('@');
    root->parent = root;
    root->suffixLink = root;

    for (size_t i = 0; i < templates.size(); ++i) {
        auto currNode = root;
        for (const auto sym : templates[i]) {
            if (currNode->children.find(sym) == currNode->children.end()) {
                auto node = std::make_shared<Node>(sym, currNode);
                currNode->children[sym] = node;
            }
            currNode = currNode->children[sym];
        }
        currNode->patternIndex = i;
        currNode->terminated = true;
    }
}

std::shared_ptr<Node> StringTemplate::mainTransition(const std::shared_ptr<Node> &node, char c) {
    if (node->children.find(c) != node->children.end()) {
        return node->children[c];
    }
    if (node == root) {
        return root;
    }

    return mainTransition(suffixLinkNode(node).lock(), c);
}

std::weak_ptr<Node> StringTemplate::suffixLinkNode(const std::shared_ptr<Node> &node) {
    if (!node->suffixLink.lock()) {
        if (node->parent.lock() == root) {
            node->suffixLink = root;

        } else {
            node->suffixLink = mainTransition(suffixLinkNode(node->parent.lock()).lock(), node->symbol);
        }
    }

    return node->suffixLink;
}


std::weak_ptr<Node> StringTemplate::shortLinkNode(const std::shared_ptr<Node> &node) {
    if (!node->shortLink.lock()) {
        auto suffix = suffixLinkNode(node);
        if (suffix.lock()->terminated || suffix.lock() == root) {
            node->shortLink = suffix;
        } else {
            node->shortLink = shortLinkNode(suffix.lock());
        }
    }
    return node->shortLink;
}


void StringTemplate::splitTemplateByQuestions(const std::string &tempString) {
    std::string accString;
    templatesCount = 0;
    templateSize = tempString.size();
    for (size_t i = 0; i < tempString.size(); ++i) {
        char sym = tempString[i];
        if (sym != '?') {
            accString += sym;
        }
        if (!accString.empty() && sym == '?') {
            templates.emplace_back(accString);
            tempPositions[accString].emplace_back(i);
            ++templatesCount;
            accString = "";
        }
    }
    if (!accString.empty()) {
        tempPositions[accString].emplace_back(tempString.size());
        ++templatesCount;
    }
}

void StringTemplate::parseText(const std::string &text) {
    auto currNode = root;
    std::vector<size_t> tempBeginPosition(text.size(), 0);
    for (size_t i = 0; i < text.size(); ++i) {
        char sym = text[i];
        currNode = mainTransition(currNode, sym);
        auto suffixNode = currNode;
        while (suffixNode != root) {
            if (suffixNode->terminated) {
                for (const auto match: tempPositions[templates[suffixNode->patternIndex]]) {
                    if (i + 1 >= match) {
                        ++tempBeginPosition[i + 1 - match];
                    }
                }
            }
            suffixNode = shortLinkNode(suffixNode).lock();
        }
    }
    for (size_t i = 0; templateSize + i <= tempBeginPosition.size(); ++i) {
        if (tempBeginPosition[i] >= templatesCount) {
            std::cout << i << ' ';
        }
    }
}

StringTemplate::StringTemplate(const std::string &tempString) {
    splitTemplateByQuestions(tempString);
    for (const auto &temp: tempPositions) {
        templates.emplace_back(temp.first);
    }
    buildTree();
}
