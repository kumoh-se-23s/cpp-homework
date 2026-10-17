#include "MyString.h"

//배열 초기화
MyString::MyString() {
    arrLength = 0;
    charArr[arrLength] = '\0';
}

MyString::MyString(char inputCharArr[]) {
    int index = 0;

    while(inputCharArr[index] != '\0') {
        appendChar(inputCharArr[index]);
        index++;
    }
}


MyString::MyString(const MyString &srcStr) {
    for(int index = 0 ; index < srcStr.length() ; index++) {
        charArr[index] = srcStr.at(index);
        arrLength++;
    }
}

int MyString::length() const {
    return CharSequence::length();
}

bool MyString::empty() const {
    return CharSequence::empty();
}

char MyString::at(int pos) const {
    return CharSequence::at(pos);
}

int MyString::find(const char subStr[], int pos) const {
    return CharSequence::find(subStr, pos);
}

int MyString::find(const MyString &subStr, int pos) const {
    return CharSequence::find(subStr, pos);
}

MyString MyString::substr(int pos, int len) {
    MyString myString = MyString();

    pos = pos >= 0 ? pos : 0;

    if(pos <= arrLength) {
        for(int index = pos ; index < arrLength && index < pos + len ; index++) {
            if(charArr[index] != '\0') {
                myString.appendChar(charArr[index]);
            }
        }
    } 
    return myString;
}

const MyString MyString::operator+(const char str2nd[]) {
    MyString resultMyString = MyString();
    resultMyString.fillArray(charArr);

    int inputIndex{0};

    for(int index = arrLength ; str2nd[inputIndex] != '\0' && index < CAPACITY ; index++) {
        resultMyString.appendChar(str2nd[inputIndex]);
        inputIndex++;
    }

    return resultMyString;
}

const MyString MyString::operator+(const MyString& str2nd) {
    MyString resultMyString = MyString();
    resultMyString.fillArray(charArr);

    int inputIndex{0};

    for(int index = arrLength ; str2nd.at(inputIndex) != '\0' && index < CAPACITY ; index++) {
        resultMyString.appendChar(str2nd.at(inputIndex));
        inputIndex++;
    }

    return resultMyString;
}

MyString& MyString::operator=(const char str2nd[]) {
    int index{0};
    arrLength = 0;

    while(arrLength < 15 && str2nd[index] != '\0') {
        charArr[index] = str2nd[index];
        arrLength++;
        index++;
    }
    
    return *this;
}

char MyString::operator[](int pos) {
    return charArr[pos];
}

bool MyString::operator==(const MyString &str) {
    if(str.arrLength != arrLength) {
        return false;
    }

    for(int index = 0 ; index < arrLength ; index++) {
        if(charArr[index] != str.at(index)) {
            return false;
        }
    }
    return true;
}   

bool MyString::operator!=(const MyString &str) {
    if(str.arrLength != arrLength) {
        return true;
    }

    for(int index = 0 ; index < arrLength ; index++) {
        if(charArr[index] != str.at(index)) {
            return true;
        }
    }
    return false;
}   