#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "Node.h"


class StringTemplate {
public:
    explicit StringTemplate(const std::string &tempString);

    std::vector<size_t> parseText(const std::string &text);

private:
    std::shared_ptr<Node> root;

    std::map<std::string, std::vector<size_t>> tempPositions;
    std::vector<std::string> templates;
    size_t templatesCount = 0;
    size_t templateSize = 0;

    void splitTemplateByQuestions(const std::string &tempString);

    void buildTree();

    std::shared_ptr<Node> mainTransition(const std::shared_ptr<Node> &node, char c);

    std::weak_ptr<Node> suffixLinkNode(const std::shared_ptr<Node> &node);

    std::weak_ptr<Node> shortLinkNode(const std::shared_ptr<Node> &node);

};
