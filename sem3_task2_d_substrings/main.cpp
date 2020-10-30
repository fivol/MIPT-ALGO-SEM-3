#include <iostream>
#include <string>
#include <vector>


const int ALPHABET_SIZE = 256;
const char END_SYMBOL = '$';


class SuffixArray {
    std::string _text;
    std::vector<int> _suffixArray;
    std::vector<int> _lsp;
public:
    explicit SuffixArray(const std::string &text) : _text(text + END_SYMBOL) {
        size_t n = _text.size();
        // Выполняем сортировку подсчетом
        // count[c] - количество буквы c
        std::vector<int> count(ALPHABET_SIZE, 0);
        std::vector<int> lsp(n, 0);
        // На каком месте в упорядоченном массиве стояла бы подстрока
        std::vector<int> substrOrderedIndex(n);
        // Класс эквивалентности данной подстроки. Если класс меньше, то строка лексикографически меньше
        std::vector<int> classes(n);
        for (auto c: _text) {
            ++count[c];
        }
        for (size_t i = 1; i < ALPHABET_SIZE; ++i) {
            count[i] += count[i - 1];
        }
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            substrOrderedIndex[--count[_text[i]]] = i;
        }
        classes[substrOrderedIndex[0]] = 0;
        int classesIndex = 0;
        for (size_t i = 1; i < n; ++i) {
            if (_text[substrOrderedIndex[i]] != _text[substrOrderedIndex[i - 1]]) {
                ++classesIndex;
            }
            classes[substrOrderedIndex[i]] = classesIndex;
        }
        std::vector<int> secondOrderedPair(n);
        std::vector<int> newClasses(n);

        for (size_t power = 0; (1U << power) <= n; ++power) {
            size_t shift = 1U << power;
            for (int i = 0; i < n; ++i) {
                secondOrderedPair[i] = substrOrderedIndex[i] - shift;
                secondOrderedPair[i] += n * (secondOrderedPair[i] < 0);
            }
            std::fill(count.begin(), count.end(), 0);
            for (int i = 0; i < n; ++i) {
                ++count[classes[i]];
            }
            for (int i = 1; i < ALPHABET_SIZE; ++i) {
                count[i] += count[i - 1];
            }

            for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
                substrOrderedIndex[--count[classes[secondOrderedPair[i]]]] = secondOrderedPair[i];
            }
            classesIndex = 0;
            newClasses[substrOrderedIndex[0]] = 0;
            std::vector<int> newLsp(n);
            for (int i = 1; i < n; ++i) {
                size_t firstCurr = substrOrderedIndex[i];
                size_t firstLast = substrOrderedIndex[i - 1];
                size_t secondCurr = (firstCurr + shift) % n;
                size_t lastSecond = (firstLast + shift) % n;
                classesIndex += (classes[firstCurr] != classes[firstLast] ||
                                 classes[secondCurr] != classes[lastSecond]);

                if (classes[firstCurr] != classes[firstLast]) {
                    newLsp[i - 1] = lsp[i - 1];
                } else {
                    newLsp[i - 1] = shift;
                    for (int j = 0; j < shift; ++j) {
                        if (_text[secondCurr + j] == _text[lastSecond + j]) {
                            newLsp[i - 1]++;
                        } else {
                            break;
                        }
                    }
                }
                newClasses[substrOrderedIndex[i]] = classesIndex;
            }
            lsp = newLsp;
            classes = newClasses;
        }
        _suffixArray.insert(_suffixArray.begin(), substrOrderedIndex.begin() + 1, substrOrderedIndex.end());
        _lsp = lsp;
    };

    [[nodiscard]] const std::vector<int> &getSuffixArray() const {
        return _suffixArray;
    }

    [[nodiscard]] const std::vector<int> &getLSP() const {
        return _lsp;
    }
};

int main() {
    std::string s;
    std::cin >> s;

    SuffixArray array(s);
    int answer = 0;

    auto lsp = array.getLSP();

    for (int i = 0; i < lsp.size() - 1; ++i) {
        answer += i + 1 - lsp[i];
    }

    std::cout << answer;

    return 0;
}
