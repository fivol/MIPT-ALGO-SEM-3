#include <string>
#include <vector>

using namespace std;

typedef long long ll;


ll palindromeSubstringsAmount(const string &text) {
    int leftBorder = 0, rightBorder = -1;
    vector<int> palindromeSize1(text.size(), 0);
    for (int i = 0; i < text.size(); ++i) {
        if (i < rightBorder)
            palindromeSize1[i] = min(rightBorder - i, palindromeSize1[leftBorder + rightBorder - i]);
        while (i + palindromeSize1[i] < text.size() && i - palindromeSize1[i] >= 0 &&
               text[i + palindromeSize1[i]] == text[i - palindromeSize1[i]])
            ++palindromeSize1[i];
        if (i + palindromeSize1[i] > rightBorder) {
            rightBorder = i + palindromeSize1[i];
            leftBorder = i - palindromeSize1[i];
        }
    }
    leftBorder = 0, rightBorder = - 1;
    vector<int> palindromeSize2(text.size(), 0);
    for (int i = 1; i < text.size(); ++i) {
        if (i < rightBorder)
            palindromeSize2[i] = min(rightBorder - i, palindromeSize2[leftBorder + rightBorder - i]);
        while (i + palindromeSize2[i] < text.size() && i - palindromeSize2[i] - 1 >= 0 &&
               text[i + palindromeSize2[i]] == text[i - palindromeSize2[i] - 1])
            ++palindromeSize2[i];
        if (i + palindromeSize2[i] > rightBorder) {
            rightBorder = i + palindromeSize2[i];
            leftBorder = i - palindromeSize2[i];
        }
    }

    ll amount = 0;
    for (ll count: palindromeSize1)
        amount += count - 1;
    for (ll count: palindromeSize2)
        amount += count;

    return amount;
}
