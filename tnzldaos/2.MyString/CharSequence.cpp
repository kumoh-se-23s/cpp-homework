#include "CharSequence.h"
#include <cstring>

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
    int subStrIndex = 0;

    for(int index = pos >= 0 ? pos : 0 ; index < arrLength ; index++) {
        if(charArr[index] == subStr[subStrIndex]) {
            subStrIndex++;
        }
        else {
            subStrIndex = 0;
        }
        
        if(subStrIndex == strlen(subStr)) {
            return index - subStrIndex + 1;
        }
    }
    return -1;
}

int CharSequence::find(const CharSequence &sequence, int pos) const {
    int subStrIndex = 0;

    for(int index = pos >= 0 ? pos : 0 ; index < arrLength ; index++) {
        if(charArr[index] == sequence.at(subStrIndex)) {
            subStrIndex++;
        }
        else {
            subStrIndex = 0;
        }
        
        if(subStrIndex == sequence.length()) {
            return index - subStrIndex + 1;
        }
    }
    return -1;
}