#include "CharSequence.h"

CharSequence::~CharSequence() {

}

void CharSequence::fillArray(char inputArr[]) {
    for (int index = 0; inputArr[index] != '\0' && index < MY_STRING_SIZE; ++index) {
        this->charArr[index] = inputArr[index];
    }
}

int CharSequence::length() const {
    return stringLength;
}

bool CharSequence::empty() const {
    return stringLength == 0;
}

char CharSequence::at(int pos) const {
    return '\0';
}

int CharSequence::find(const char subStr[], int pos) const {
    while() {

    }
}

int CharSequence::find(const CharSequence &sequence, int pos) {
    
}

