#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "./Node.cpp"

class StringTemplate {
public:
    void compileTemplate(const std::string &tempString);

    void parseText(const std::string &text);

private:
    std::shared_ptr<Node> root;

    std::map<std::string, std::vector<size_t>> tempPositions;
    std::vector<std::string> templates;
    size_t templatesCount = 0;
    size_t templateSize = 0;

    void splitTemplateByQuestions(const std::string &tempString);

    void buildTree();

    std::shared_ptr<Node> mainTransition(const std::shared_ptr<Node> &node, char c);

    std::shared_ptr<Node> suffixLinkNode(const std::shared_ptr<Node> &node);

    std::shared_ptr<Node> shortLinkNode(const std::shared_ptr<Node> &node);

};

void StringTemplate::buildTree() {
    root = std::make_shared<Node>();
    root->parent = root;
    root->suffixLink = root;

    for (int i = 0; i < templates.size(); ++i) {
        auto currNode = root;
        for (auto sym : templates[i]) {
            if (currNode->children.find(sym) == currNode->children.end()) {
                auto node = std::make_shared<Node>();
                node->symbol = sym;
                node->parent = currNode;
                currNode->children[sym] = node;
            }
            currNode = currNode->children[sym];
        }
        currNode->patternIndex = i;
        currNode->terminated = true;
    }
}

std::shared_ptr<Node> StringTemplate::mainTransition(const std::shared_ptr<Node> &node, char c) {
    if (node->children.find(c) != node->children.end())
        return node->children[c];

    if (node == root)
        return root;

    return mainTransition(suffixLinkNode(node), c);
}

std::shared_ptr<Node> StringTemplate::suffixLinkNode(const std::shared_ptr<Node> &node) {
    if (!node->suffixLink.lock()) {
        if (node->parent.lock() == root)
            node->suffixLink = root;
        else
            node->suffixLink = mainTransition(suffixLinkNode(node->parent.lock()), node->symbol);
    }

    return node->suffixLink.lock();
}


std::shared_ptr<Node> StringTemplate::shortLinkNode(const std::shared_ptr<Node> &node) {
    if (!node->shortLink.lock()) {
        auto suffix = suffixLinkNode(node);
        if (suffix->terminated || suffix == root)
            node->shortLink = suffix;
        else
            node->shortLink = shortLinkNode(suffix);
    }
    return node->shortLink.lock();
}

void StringTemplate::compileTemplate(const std::string &tempString) {
    splitTemplateByQuestions(tempString);
    for (auto &temp: tempPositions) {
        templates.emplace_back(temp.first);
    }
    buildTree();
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
            templates.push_back(accString);
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
    for (int i = 0; i < text.size(); ++i) {
        char sym = text[i];
        currNode = mainTransition(currNode, sym);
        auto suffixNode = currNode;
        while (suffixNode != root) {
            if (suffixNode->terminated) {
                for (auto match: tempPositions[templates[suffixNode->patternIndex]]) {
                    int pos = (int) i + 1 - (int) match;
                    if (pos >= 0)
                        ++tempBeginPosition[pos];
                }
            }
            suffixNode = shortLinkNode(suffixNode);
        }
    }
    for (int i = 0; i < (int) tempBeginPosition.size() - (int) templateSize + 1; ++i) {
        if (tempBeginPosition[i] >= templatesCount)
            std::cout << i << ' ';
    }
}
