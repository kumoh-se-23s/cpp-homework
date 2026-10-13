#include "CharSequence.h"
#include <cstring>

CharSequence::~CharSequence() {

}

void CharSequence::fillArray(char inputArr[]) {
    for (int index = 0; inputArr[index] != '\0' && index < CAPACITY; ++index) {
        this->charArr[index] = inputArr[index];
    }
}

void CharSequence::setChar(char inputChar, int pos) {
    charArr[pos] = inputChar;
}

int CharSequence::length() const {
    return arrLength;
}

bool CharSequence::empty() const {
    return arrLength == 0;
}

char CharSequence::at(int pos) const {
    if(pos > arrLength || pos < 0) {
        return '\0';
    }
    else {
        return charArr[pos];
    }
}

int CharSequence::find(const char subStr[], int pos) const {
    int subStrIndex = 0;
    //int correctCount = 0;

    for(int index = pos >= 0 ? pos : 0 ; index < arrLength ; index++) {
        if(charArr[index] == subStr[subStrIndex]) {
            subStrIndex++;
        }
        else {
            subStrIndex = 0;
        }
        
        if(subStrIndex == strlen(subStr)) {
            return index - subStrIndex;
        }
    }
    return -1;
}

int CharSequence::find(const CharSequence &sequence, int pos) {
    
}

