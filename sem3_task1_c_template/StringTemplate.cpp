#include <memory>
#include <map>
#include <string>
#include <vector>
#include "StringTemplate.h"


void StringTemplate::buildTree() {
    root = std::make_shared<StringTemplate::Node>(root, root);

    for (size_t i = 0; i < templates.size(); ++i) {
        auto currNode = root;
        for (const auto sym : templates[i]) {
            if (currNode->children.find(sym) == currNode->children.end()) {
                auto node = std::make_shared<StringTemplate::Node>(sym, currNode);
                currNode->children[sym] = node;
            }
            currNode = currNode->children[sym];
        }
        currNode->patternIndex = i;
        currNode->finite = true;
    }
}

std::shared_ptr<StringTemplate::Node> StringTemplate::mainTransition(const std::shared_ptr<StringTemplate::Node> &node, char c) {
    if (node->children.find(c) != node->children.end()) {
        return node->children[c];
    }
    if (node == root) {
        return root;
    }

    return mainTransition(suffixLinkNode(node).lock(), c);
}

std::weak_ptr<StringTemplate::Node> StringTemplate::suffixLinkNode(const std::shared_ptr<StringTemplate::Node> &node) {
    if (node->suffixLink.expired()) {
        if (node->parent.lock() == root) {
            node->suffixLink = root;

        } else {
            node->suffixLink = mainTransition(suffixLinkNode(node->parent.lock()).lock(), node->symbol);
        }
    }

    return node->suffixLink;
}


std::weak_ptr<StringTemplate::Node> StringTemplate::shortLinkNode(const std::shared_ptr<StringTemplate::Node> &node) {
    if (node->shortLink.expired()) {
        auto suffix = suffixLinkNode(node);
        if (suffix.lock()->finite || suffix.lock() == root) {
            node->shortLink = suffix;
        } else {
            node->shortLink = shortLinkNode(suffix.lock());
        }
    }
    return node->shortLink;
}


void StringTemplate::splitTemplateByQuestions(const std::string &tempString, char separator) {
    std::string accString;
    templatesCount = 0;
    templateSize = tempString.size();
    for (size_t i = 0; i < tempString.size(); ++i) {
        char sym = tempString[i];
        if (sym != separator) {
            accString += sym;
        }
        if (!accString.empty() && sym == separator) {
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

std::vector<size_t> StringTemplate::parseText(const std::string &text) {
    auto currNode = root;
    std::vector<size_t> tempBeginPosition(text.size());
    for (size_t i = 0; i < text.size(); ++i) {
        char sym = text[i];
        currNode = mainTransition(currNode, sym);
        auto suffixNode = currNode;
        while (suffixNode != root) {
            if (suffixNode->finite) {
                for (const auto match: tempPositions[templates[suffixNode->patternIndex]]) {
                    if (i + 1 >= match) {
                        ++tempBeginPosition[i + 1 - match];
                    }
                }
            }
            suffixNode = shortLinkNode(suffixNode).lock();
        }
    }
    std::vector<size_t> templateMatches;
    for (size_t i = 0; templateSize + i <= tempBeginPosition.size(); ++i) {
        if (tempBeginPosition[i] >= templatesCount) {
            templateMatches.emplace_back(i);
        }
    }
    return templateMatches;
}

StringTemplate::StringTemplate(const std::string &tempString) {
    splitTemplateByQuestions(tempString);
    for (const auto &temp: tempPositions) {
        templates.emplace_back(temp.first);
    }
    buildTree();
}
