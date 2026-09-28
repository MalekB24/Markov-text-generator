# CIS 25: Markov Chain Text Generator

## Files
- main.cpp: prompts, input validation, generation report.
- markov.h: the six required declarations and include guard.
- markov.cpp: the six implementations using fixed-capacity arrays.
- test.txt: the cat-story example from the assignment.

## Compile and run
Open a terminal in this folder in a C++ environment (for example your class Codespace).

```sh
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp markov.cpp -o markov
./markov
```

On Windows with g++, use `./markov.exe` instead. Enter `test.txt`, an order from 1 to 3, and a maximum output count at least as large as the order. Full input paths containing spaces work; do not surround the typed path with quotes.

The program reads at most 5000 whitespace-separated words. Capitalization and punctuation are preserved. It keeps duplicate transitions so common successors receive more chances of selection. The initial prefix counts toward the output limit. At a dead end it stops instead of restarting. Different runs may legitimately produce identical text.

## Understand the six functions
1. joinWords combines a consecutive range with single spaces.
2. readWordsFromFile fills an array within its capacity; -1 means the file could not be opened.
3. buildMarkovChain records each prefix and the next word in parallel arrays.
4. getRandomSuffix counts matching entries, selects an entry, and returns its suffix.
5. getRandomPrefix selects a starting entry, guarding the empty case.
6. generateText follows recorded transitions and slides its prefix window after each word.

The caller supplies valid array ranges and capacities. generateText expects matching prefix/suffix arrays produced by buildMarkovChain with the same order.

## Authorship and Git
This implementation was created with AI assistance (Codex). The included Git history records actual implementation milestones by Codex; it is not a record of independent student work. Review and understand the implementation and follow your professor's disclosure requirements.

## Submit
The PDF requires a GitHub repository URL, not just a ZIP file. The local project has Git history but no remote configured. Create or use your course repository, preserve this history when publishing, push the main branch, and ensure the professor can access it. Submit that repository URL in Canvas. No GitHub publication or Canvas submission has been performed here.

If creating an EMPTY GitHub repository, run these inside this folder, replacing the example URL:
```sh
git remote add origin https://github.com/YOUR-USERNAME/YOUR-REPOSITORY.git
git push -u origin main
```
Do not use those commands unchanged if your course repository already has files/history.
