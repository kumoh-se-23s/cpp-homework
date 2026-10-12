#pragma once
#include<iostream>

using namespace std;

class TextFormatter {
public:
    static void countAlphabet(const char base[], int result[26]);

    static void capitalizeFirstLetter(const char base[], char result[]);

    static void encryptSentence(const char base[], char result[], int gap);

    static char toUpper(char chr);

    static char toLower(char chr);

    static bool isAlphabet(char chr);

    static bool isDigit(char chr);

    constexpr static int MAX_OF_ALPHABETS = 26;

    constexpr static  int MAX_OF_DIGITS = 10;

private:
    static int getFirstLetterIndex(const char str[]);

    static char getGapChar(char chr, int gap, int maxLength);

};






