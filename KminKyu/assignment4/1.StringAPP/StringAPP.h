#pragma once

class StringAPP {
public:
    StringAPP();

    void readInput();

    void printAlphabetCounts() const;

    void printCapitalized() const;

    void printEncryptedSentence() const;

    void run();

private:
    constexpr static int MAX_STR_SIZE = 128;

    char inputCharArray[MAX_STR_SIZE] = {};

    int gap = 0;
};


