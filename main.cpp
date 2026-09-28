#include "markov.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <limits>

// Read a whole line so inputs such as 2abc or 2.5 are rejected too.
bool readInteger(const std::string& prompt, int minimum, int maximum, int& value) {
    std::string line;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, line)) return false;
        std::istringstream input(line);
        char extra;
        if ((input >> value) && !(input >> extra) && value >= minimum && value <= maximum)
            return true;
        std::cout << "Please enter a whole number from " << minimum
                  << " to " << maximum << ".\n";
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    const int MAX_WORDS = 5000;
    std::string words[MAX_WORDS], prefixes[MAX_WORDS], suffixes[MAX_WORDS];
    std::string filename;
    int order = 1;
    int requested = 1;
    int count = 0;
    while (true) {
        std::cout << "Enter input filename: ";
        if (!std::getline(std::cin, filename)) return 0;
        if (!readInteger("Enter order (1, 2, or 3): ", 1, 3, order)) return 0;
        if (!readInteger("Enter maximum number of words to generate: ", order,
                         std::numeric_limits<int>::max(), requested)) return 0;
        count = readWordsFromFile(filename, words, MAX_WORDS);
        if (count == -1) {
            std::cout << "Could not open the file. Check its name or full path and try again.\n";
        } else if (count <= order) {
            std::cout << "Not enough training words: need at least " << order + 1
                      << ", but read " << count << ". Try another file or order.\n";
        } else break;
    }
    if (count == MAX_WORDS)
        std::cout << "At most " << MAX_WORDS
                  << " input words were used; additional words, if any, were ignored.\n";
    int chainSize = buildMarkovChain(words, count, order, prefixes, suffixes, MAX_WORDS);
    if (chainSize <= 0) {
        std::cerr << "No transitions are available for generation.\n";
        return 1;
    }
    std::string output = generateText(prefixes, suffixes, chainSize, order, requested);
    std::cout << '\n' << output << '\n';
    std::istringstream generated(output);
    std::string word;
    int actual = 0;
    while (generated >> word) ++actual;
    std::cout << "Generated " << actual << " of at most " << requested << " words.\n";
    if (actual < requested)
        std::cout << "Stopped early: the current prefix has no successor.\n";
    return 0;
}
