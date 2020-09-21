#include <string>
#include <vector>


int64_t palindromeSubstringsAmount(const std::string &text) {
    size_t leftBorderOdd = 0, rightBorderOdd = 0;
    size_t leftBorderEven = 0, rightBorderEven = 0;
    std::vector<size_t> palindromeSizeOdd(text.size());
    std::vector<size_t> palindromeSizeEven(text.size());
    for (size_t i = 0; i < text.size(); ++i) {
        if (i < rightBorderOdd){
            palindromeSizeOdd[i] = std::min(rightBorderOdd - i, palindromeSizeOdd[leftBorderOdd + rightBorderOdd - i]);
        }
        if (i < rightBorderEven){
            palindromeSizeEven[i] = std::min(rightBorderEven - i, palindromeSizeEven[leftBorderEven + rightBorderEven - i]);
        }
        while (i + palindromeSizeOdd[i] < text.size() && i - palindromeSizeOdd[i] >= 0 &&
               text[i + palindromeSizeOdd[i]] == text[i - palindromeSizeOdd[i]]){
            ++palindromeSizeOdd[i];
        }
        while (i + palindromeSizeEven[i] < text.size() && i - palindromeSizeEven[i] - 1 >= 0 &&
               text[i + palindromeSizeEven[i]] == text[i - palindromeSizeEven[i] - 1]){
            ++palindromeSizeEven[i];
        }

        if (i + palindromeSizeOdd[i] > rightBorderOdd) {
            rightBorderOdd = i + palindromeSizeOdd[i];
            leftBorderOdd = i - palindromeSizeOdd[i];
        }

        if (i + palindromeSizeEven[i] > rightBorderEven) {
            rightBorderEven = i + palindromeSizeEven[i];
            leftBorderEven = i - palindromeSizeEven[i];
        }
    }

    int64_t amount = 0;
    for (auto count: palindromeSizeOdd){
        amount += count - 1;
    }

    for (auto count: palindromeSizeEven){
        amount += count;
    }

    return amount;
}
