#include <string>
#include <vector>

using namespace std;

void calcStringZFunction(vector<int> &zFuncValues, const string &text) {
    zFuncValues.resize(text.size());
    zFuncValues[0] = 0;
    int lBorder = 0;
    int rBorder = 0;
    for (int i = 1; i < zFuncValues.size(); ++i) {
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

void findPatternPositions(vector<int> &patternPositions, const string &text, const string &pattern) {
    vector<int> zFuncValues;
    string searchPattern = pattern + '@' + text;
    calcStringZFunction(zFuncValues, searchPattern);
    for (int i = pattern.size(); i < zFuncValues.size(); ++i) {
        if (zFuncValues[i] == pattern.size())
            patternPositions.push_back(i - pattern.size() - 1);
    }
}
