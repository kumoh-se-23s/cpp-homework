#include "MyString.h"

MyString::MyString() {
    capacity = 4;
    size = 0;
    stringArr = new char[capacity];
}

MyString::MyString(const MyString& srcStr) {
    capacity = 4;
    size = 0;
    stringArr = new char[capacity];

    for(int index = 0 ; index < srcStr.size ; index++) {
        this->stringArr[index] = srcStr.stringArr[index];
    } 
}

MyString::~MyString() {
    delete[] stringArr;
}

void MyString::resize() {
    if(size != capacity - 1) {
        return;
    }

    capacity = capacity * 2;
    char* newStringArr = new char[capacity];

    for(int index = 0 ; index < size ; index++) {
        newStringArr[index] = stringArr[index];
    }
    
    delete[] stringArr;
    stringArr = newStringArr;
}

void MyString::append(char inputChar) {
    resize();

    stringArr[size] = inputChar;
    size++;
}

//----------------------------------------------------------------------

int MyString::length() const {
    return size;
}

bool MyString::isEmpty() const {
    return size == 0;
}

int MyString::getCapacity() const {
    return capacity;
}

char& MyString::at(int pos) {
    return stringArr[pos];
}

char& MyString::operator[](int pos) {
    return stringArr[pos];
}

const char& MyString::operator[](int pos) const {
    return stringArr[pos];
}

MyString MyString::operator=(const MyString& srcStr) {
    if(&srcStr == this) {
        return *this;
    }

    delete[] stringArr;

    for(int index = 0 ; index < srcStr.size ; index++) {
        append(srcStr.stringArr[index]);
    }
    return *this;
}

//--------------------------------------------------------------------

ostream& operator<<(ostream& outputStream, const MyString& str) {
    for(int index = 0 ; index < str.length() ; index++) {
        outputStream << str[index];
    }

    return outputStream; 
}

istream& operator>>(istream& inputStream, MyString& str) {

}

istream& getline(istream& inputStream, MyString& str, char delim = '\n') {

}