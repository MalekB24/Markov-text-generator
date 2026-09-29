# Markov Chain Text Generator

A C++ project that reads a text file and generates text using the word patterns it finds.

The user chooses the input file, an order from 1 to 3, and the maximum number of output words. Generation stops at the word limit or when no next word is available.

## Compile and run

g++ -std=c++11 -Wall -Wextra -pedantic main.cpp markov.cpp -o markov.exe

Then run: .\markov.exe

## Testing

I compiled the program and manually tested all three orders using test.txt.

## Acknowledgment

Built with AI assistance; I compiled and tested the program.