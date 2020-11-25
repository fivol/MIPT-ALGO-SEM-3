#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>


class StringTemplate {
public:
    explicit StringTemplate(const std::string &tempString);

    std::vector<size_t> parseText(const std::string &text);

private:

    struct Node {
        Node(char symbol, std::shared_ptr<Node> &parent) : symbol(symbol), parent(parent) {};

        Node(std::shared_ptr<Node> &parent, std::shared_ptr<Node> &suffixLink) : parent(parent),
                                                                                 suffixLink(suffixLink) {};

        std::weak_ptr<Node> parent;
        std::map<char, std::shared_ptr<Node>> children;
        std::weak_ptr<Node> suffixLink;
        std::weak_ptr<Node> shortLink;
        size_t patternIndex = 0;
        bool finite = false;
        char symbol{};
    };

    std::shared_ptr<Node> root;

    std::map<std::string, std::vector<size_t>> tempPositions;
    std::vector<std::string> templates;
    size_t templatesCount = 0;
    size_t templateSize = 0;

    void splitTemplateByQuestions(const std::string &tempString, char separator = '?');

    void buildTree();

    std::shared_ptr<Node> mainTransition(const std::shared_ptr<Node> &node, char c);

    std::weak_ptr<Node> suffixLinkNode(const std::shared_ptr<Node> &node);

    std::weak_ptr<Node> shortLinkNode(const std::shared_ptr<Node> &node);

};
