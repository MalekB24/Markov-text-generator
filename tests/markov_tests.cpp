#include "markov.h"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
    std::srand(17);
    std::string words[] = {"a", "b", "a", "b", "c", "end."};
    std::string p[10], s[10];
    assert(joinWords(words, 1, 3) == "b a b");
    assert(joinWords(words, 0, 0).empty());
    assert(buildMarkovChain(words, 6, 0, p, s, 10) == 0);
    assert(buildMarkovChain(words, 3, 3, p, s, 10) == 0);
    assert(buildMarkovChain(words, 6, 1, p, s, 0) == 0);
    p[1] = "sentinel";
    assert(buildMarkovChain(words, 6, 1, p, s, 1) == 1);
    assert(p[1] == "sentinel");
    assert(getRandomPrefix(p, 0).empty());
    assert(getRandomSuffix(p, s, 0, "a").empty());
    assert(generateText(p, s, 0, 1, 20).empty());
    assert(generateText(p, s, 1, 4, 20).empty());
    assert(generateText(p, s, 1, 2, 1).empty());
    for (int order = 1; order <= 3; ++order) {
        int n = buildMarkovChain(words, 6, order, p, s, 10);
        assert(n == 6 - order);
        for (int i = 0; i < n; ++i) {
            assert(p[i] == joinWords(words, i, order));
            assert(s[i] == words[i + order]);
        }
        assert(getRandomSuffix(p, s, n, "missing").empty());
        for (int run = 0; run < 100; ++run) {
            std::string output = generateText(p, s, n, order, 30);
            std::istringstream input(output);
            std::vector<std::string> out;
            std::string word;
            while (input >> word) out.push_back(word);
            assert(out.size() >= static_cast<unsigned>(order) && out.size() <= 30);
            for (unsigned i = 0; i + order < out.size(); ++i) {
                bool found = false;
                std::string prefix = joinWords(out.data(), i, order);
                for (int j = 0; j < n; ++j)
                    if (p[j] == prefix && s[j] == out[i + order]) found = true;
                assert(found);
            }
            if (out.size() < 30)
                assert(getRandomSuffix(p, s, n, joinWords(out.data(), out.size()-order, order)).empty());
            std::istringstream exact(generateText(p, s, n, order, order));
            int count = 0;
            while (exact >> word) ++count;
            assert(count == order);
        }
    }
    std::string oneP[] = {"start"}, oneS[] = {"finish"};
    assert(generateText(oneP, oneS, 1, 1, 20) == "start finish");
    std::ofstream("fixture.txt") << "Hello,  WORLD!\nnext";
    std::string read[3] = {"", "guard", ""};
    assert(readWordsFromFile("fixture.txt", read, 1) == 1);
    assert(read[0] == "Hello," && read[1] == "guard");
    assert(readWordsFromFile("fixture.txt", read, 0) == 0);
    assert(readWordsFromFile("fixture.txt", read, -1) == 0);
    assert(readWordsFromFile("fixture.txt", read, 3) == 3);
    assert(read[1] == "WORLD!");
    std::ofstream("fixture.txt") << " \n\t";
    assert(readWordsFromFile("fixture.txt", read, 3) == 0);
    assert(readWordsFromFile("not-present-12345.txt", read, 3) == -1);
    std::cout << "All function checks passed, including 300 generated transition traces.\n";
}
