#include "MyString.h"

MyString::MyString() {

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

int MyString::find(const char subStr[], int pos) {
    
}

int MyString::find(const MyString &subStr, int pos) {
    
}

MyString MyString::substr(int pos, int len) {
    
}

const MyString MyString::operator+(const char str2nd[]) {
    MyString resultMyString = MyString();
    resultMyString.fillArray(charArr);

    int inputIndex{0};

    for(int index = stringLength ; index < MY_STRING_SIZE ; index++) {
        resultMyString.charArr[index] = str2nd[inputIndex];
        inputIndex++;
    }
}

const MyString MyString::operator+(const MyString& str2nd) {
    MyString resultMyString = MyString();
    resultMyString.fillArray();

    int inputIndex{0};

    for(int index = stringLength ; index < MY_STRING_SIZE ; index++) {
        resultMyString;
        inputIndex++;
    }
}

void MyString::operator=(const MyString &srcStr) {
    
}

MyString& MyString::operator=(const char str2nd[]) {

}

char MyString::operator[](int pos) {
    
}

bool MyString::operator==(const MyString &str) {
    
}

bool MyString::operator!=(const MyString &str) {
    
}