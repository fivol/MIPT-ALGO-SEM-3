#include <string>
#include <vector>

void calcStringZFunction(std::vector<size_t> &zFuncValues, const std::string &text) {
    zFuncValues.resize(text.size());
    zFuncValues[0] = 0;
    size_t lBorder = 0;
    size_t rBorder = 0;
    for (size_t i = 1; i < zFuncValues.size(); ++i) {
        if (i < rBorder && zFuncValues[i - lBorder] + i < rBorder) {
            zFuncValues[i] = zFuncValues[i - lBorder];
        } else {
            if (i < rBorder)
                zFuncValues[i] = rBorder - i;
            while (i + zFuncValues[i] < text.size() && text[zFuncValues[i]] == text[i + zFuncValues[i]])
                ++zFuncValues[i];
            lBorder = i;
            rBorder = i + zFuncValues[i];
        }
    }
}

void findPatternPositions(std::vector<size_t> &patternPositions, const std::string &text, const std::string &pattern) {
    std::vector<size_t> zFuncValues;
    std::string searchPattern = pattern + '@' + text;
    calcStringZFunction(zFuncValues, searchPattern);
    for (size_t i = pattern.size(); i < zFuncValues.size(); ++i) {
        if (zFuncValues[i] == pattern.size())
            patternPositions.push_back(i - pattern.size() - 1);
    }
}
