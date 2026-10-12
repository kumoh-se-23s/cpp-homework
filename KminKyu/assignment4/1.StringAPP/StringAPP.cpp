#include<iostream>
#include "StringAPP.h"
#include "TextFormatter.h"

using namespace std;

StringAPP::StringAPP() = default;

void StringAPP::readInput() {
    cin.getline(inputCharArray, MAX_STR_SIZE);
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin >> gap;
}

void StringAPP::printAlphabetCounts() const {
    int result[TextFormatter::MAX_OF_ALPHABETS] = {};
    TextFormatter::countAlphabet(inputCharArray, result);
    for (int index = 0; index < TextFormatter::MAX_OF_ALPHABETS; ++index) {
        if (result[index] != 0) {
            cout << "[" << static_cast<char>(index + 'a') << ":" << result[index] << "] ";
        }
    }
    cout << "\n";
}

void StringAPP::printCapitalized() const {
    char result[MAX_STR_SIZE] = {};
    TextFormatter::capitalizeFirstLetter(inputCharArray, result);
    cout << result << "\n";
}

void StringAPP::printEncryptedSentence() const {
    char result[MAX_STR_SIZE] = {};
    TextFormatter::encryptSentence(inputCharArray, result, gap);
    cout << result << "\n";
}

void StringAPP::run() {
    readInput();
    printAlphabetCounts();
    printCapitalized();
    printEncryptedSentence();
}
