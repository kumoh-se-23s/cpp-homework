#include "MyString.h"
#include <cstring>

MyString::MyString() {
    for(int index = 0; index < strlen(charArr) ; index++) {
        charArr[index];
    }
}

MyString::MyString(const MyString &srcStr) {
    for(int index = 0 ; index < srcStr.length() ; index++) {
        charArr[index] = srcStr.at(index);
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
    
}

MyString MyString::substr(int pos, int len) {
    MyString myString = MyString();

    int inputCharArrIndex = 0;
    pos = pos >= 0 ? pos : 0;

    if(pos <= arrLength) {
        for(int index = pos ; index <= pos + len ; index++) {
            myString.setChar(charArr[index], inputCharArrIndex);
            inputCharArrIndex++;
        }
    } 
    return myString;
}

const MyString MyString::operator+(const char str2nd[]) {
    MyString resultMyString = MyString();
    resultMyString.fillArray(charArr);

    int inputIndex{0};

    for(int index = arrLength ; index < CAPACITY ; index++) {
        resultMyString.charArr[index] = str2nd[inputIndex];
        inputIndex++;
    }
}

const MyString MyString::operator+(const MyString& str2nd) {
    MyString resultMyString = MyString();
    resultMyString.fillArray(charArr);

    int inputIndex{0};

    for(int index = arrLength ; index < CAPACITY ; index++) {
        resultMyString;
        inputIndex++;
    }
}

MyString& MyString::operator=(const char str2nd[]) {

}

char MyString::operator[](int pos) {
    
}

bool MyString::operator==(const MyString &str) {
    if(arrLength != str.length()) {
        return false;
    } 
    else {
        for(int index = 0 ; index < arrLength ; index++) {
            if(charArr[index] != str.at(index)) {
                return false;
            }
        }
    }
    return true;
}

bool MyString::operator!=(const MyString &str) {
    if(arrLength != str.length()) {
        return true;
    } 
    else {
        for(int index = 0 ; index < arrLength ; index++) {
            if(charArr[index] == str.at(index)) {
                return false;
            }
        }
    }
    return true;
}   