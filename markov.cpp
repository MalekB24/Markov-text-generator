#include "markov.h"
#include <fstream>
#include <cstdlib>

std::string joinWords(const std::string words[], int startIndex, int count) {
    std::string result;
    for (int i = 0; i < count; ++i) {
        if (i > 0) result += ' ';
        result += words[startIndex + i];
    }
    return result;
}

int readWordsFromFile(std::string filename, std::string words[], int maxWords) {
    std::ifstream inputFile(filename.c_str());
    if (!inputFile.is_open()) return -1;
    int count = 0;
    // Check capacity before attempting to write the next word.
    while (count < maxWords && inputFile >> words[count]) ++count;
    inputFile.close();
    return count;
}

int buildMarkovChain(const std::string words[], int numWords, int order,
                     std::string prefixes[], std::string suffixes[], int maxChainSize) {
    if (order < 1 || order > 3 || numWords <= order || maxChainSize <= 0) return 0;
    int count = 0;
    for (int i = 0; i < numWords - order && count < maxChainSize; ++i) {
        prefixes[count] = joinWords(words, i, order);
        suffixes[count] = words[i + order];
        ++count;
    }
    // Keep duplicates: repeated observations should be more likely choices.
    return count;
}

std::string getRandomSuffix(const std::string prefixes[], const std::string suffixes[],
                            int chainSize, std::string currentPrefix) {
    int matchCount = 0;
    for (int i = 0; i < chainSize; ++i) {
        if (prefixes[i] == currentPrefix) ++matchCount;
    }
    if (matchCount == 0) return "";
    int pick = std::rand() % matchCount;
    for (int i = 0; i < chainSize; ++i) {
        if (prefixes[i] == currentPrefix) {
            if (pick == 0) return suffixes[i];
            --pick;
        }
    }
    return "";
}

std::string getRandomPrefix(const std::string prefixes[], int chainSize) {
    if (chainSize <= 0) return "";
    return prefixes[std::rand() % chainSize];
}

std::string generateText(const std::string prefixes[], const std::string suffixes[],
                         int chainSize, int order, int numWords) {
    if (chainSize <= 0 || order < 1 || order > 3 || numWords < order) return "";
    std::string currentPrefix = getRandomPrefix(prefixes, chainSize);
    std::string result = currentPrefix;
    std::string currentWords[3];
    int wordIndex = 0;
    // Prefixes produced by buildMarkovChain contain exactly 'order' words.
    for (std::string::size_type i = 0; i < currentPrefix.length(); ++i) {
        if (currentPrefix[i] == ' ') ++wordIndex;
        else currentWords[wordIndex] += currentPrefix[i];
    }
    for (int i = 0; i < numWords - order; ++i) {
        std::string newWord = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);
        if (newWord.empty()) break; // Stop at a dead end; never restart elsewhere.
        result += " " + newWord;
        for (int j = 0; j < order - 1; ++j) currentWords[j] = currentWords[j + 1];
        currentWords[order - 1] = newWord;
        currentPrefix = joinWords(currentWords, 0, order);
    }
    return result;
}
