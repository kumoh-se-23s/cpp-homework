#include<iostream>
#include "TextFormatter.h"

void TextFormatter::countAlphabet(const char base[], int result[MAX_OF_ALPHABETS]) {
    for (int index = 0; base[index] != '\0'; ++index) {
        char lowerChar = toLower(base[index]);
        if (isAlphabet(lowerChar)) {
            result[lowerChar - 'a'] += 1;
        }
    }
}

void TextFormatter::capitalizeFirstLetter(const char base[], char result[]) {
    for (int index = 0; base[index] != '\0'; ++index) {
        result[index] = toLower(base[index]);
    }
    int firstLetterIndex = getFirstLetterIndex(base);
    result[firstLetterIndex] = toUpper(result[firstLetterIndex]);

}

void TextFormatter::encryptSentence(const char base[], char result[], int gap) {
    for (int index = 0; base[index] != '\0'; ++index) {
        if (isAlphabet(base[index])) {
            result[index] = getGapChar(base[index], gap, MAX_OF_ALPHABETS);
        } else if (isDigit(base[index])) {
            result[index] = getGapChar(base[index], gap, MAX_OF_DIGITS);
        } else {
            result[index] = base[index];
        }

    }
}

char TextFormatter::toUpper(char chr) {
    if ('a' <= chr && chr <= 'z') {
        return static_cast<char>(chr - 'a' + 'A');
    }
    return chr;
}

char TextFormatter::toLower(char chr) {
    if ('A' <= chr && chr <= 'Z') {
        return static_cast<char>(chr - 'A' + 'a');
    }
    return chr;
}

bool TextFormatter::isAlphabet(char chr) {
    if ('a' <= chr && chr <= 'z') {
        return true;
    } else if (('A' <= chr && chr <= 'Z')) {
        return true;
    }
    return false;

}

bool TextFormatter::isDigit(char chr) {
    if ('0' <= chr && chr <= '9') {
        return true;
    }
    return false;
}

int TextFormatter::getFirstLetterIndex(const char str[]) {
    int index;
    for (index = 0; str[index] != '\0' && std::isspace(str[index]); ++index) {}
    return index;
}

char TextFormatter::getGapChar(char chr, int gap, int maxLength) {
    char baseChar;
    if (isDigit(chr)) {
        baseChar = '0';
    } else if ('a' <= chr && chr <= 'z') {
        baseChar = 'a';
    } else if ('A' <= chr && chr <= 'Z') {
        baseChar = 'A';
    } else {
        return chr;
    }
    return static_cast<char>(((chr - baseChar) + gap % maxLength + maxLength) % maxLength + baseChar);
}
