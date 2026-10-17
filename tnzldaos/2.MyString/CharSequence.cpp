#include "CharSequence.h"
#include "utils.h"

void CharSequence::fillArray(char inputArr[]) {
    arrLength = 0;

    for (int index = 0; index < CAPACITY && inputArr[index] != '\0'; index++) {
        charArr[index] = inputArr[index];
        arrLength++;
    }

    charArr[arrLength] = '\0';
}

CharSequence::CharSequence() {

}

CharSequence::CharSequence(char inputArr[]) {
    fillArray(inputArr);
}

//특정 인덱스에 있는 값 수정6
void CharSequence::setChar(char inputChar, int pos) {
    charArr[pos] = inputChar;
}

void CharSequence::appendChar(char inputChar) {
    if(arrLength < 15) {
        charArr[arrLength] = inputChar;
        charArr[arrLength + 1] = '\0';
        arrLength++;
    }
}

int CharSequence::length() const {
    return arrLength;
}

void CharSequence::incrementLength() {
    arrLength++;
}

bool CharSequence::empty() const {
    
    return arrLength == 0;
}

char CharSequence::at(int pos) const {
    if(pos >= arrLength || pos < 0) {
        return '\0';
    }
    else {
        return charArr[pos];
    }
}

int CharSequence::find(const char subStr[], int pos) const {
    Utils utils;

    int startIndex{pos < 0 ? 0 : pos};
    int windowSize = utils.getCharArrLength(subStr);
    
    while(startIndex + windowSize <= arrLength) {
        int subStrIndex{0};

        for(int index = startIndex ; index < startIndex + windowSize ; index++) {
            if(charArr[index] == subStr[subStrIndex]) {
                subStrIndex++;
            }
            else {
                startIndex = subStrIndex == 0 ? index + 1: index;
                break;
            }
        }

        if(subStrIndex == windowSize) {
            return startIndex;
        }
    }

    return -1;
}

int CharSequence::find(const CharSequence &sequence, int pos) const {
    Utils utils;

    int startIndex{pos < 0 ? 0 : pos};
    int windowSize = utils.getCharArrLength(sequence.charArr);
    
    while(startIndex + windowSize <= arrLength) {
        int subStrIndex{0};

        for(int index = startIndex ; index < startIndex + windowSize ; index++) {
            if(charArr[index] == sequence.charArr[subStrIndex]) {
                subStrIndex++;
            }
            else {
                startIndex = subStrIndex == 0 ? index + 1: index;
                break;
            }
        }

        if(subStrIndex == windowSize) {
            return startIndex;
        }
    }

    return -1;
}