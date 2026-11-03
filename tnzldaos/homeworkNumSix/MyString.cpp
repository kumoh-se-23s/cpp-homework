#include "MyString.h"

void MyString::resize() {
    capacity = capacity * 2;
    char* newStringArr = new char[capacity];

    for(int index = 0 ; index < size ; index++) {
        newStringArr[index] = stringArr[index];
    }
    
    delete[] stringArr;
    stringArr = newStringArr;
    stringArr[size] = '\0';
}

void MyString::append(char inputChar) {
    if(size > capacity - 2) {
        resize();
    }

    stringArr[size] = inputChar;
    size++;

    stringArr[size] = '\0';
}

MyString::MyString() {
    capacity = 8;
    size = 0;
    stringArr = new char[capacity];
    stringArr[size] = '\0';
}

MyString::MyString(const MyString& srcStr) {
    capacity = 8;
    size = 0;
    stringArr = new char[capacity];

    for(int index = 0 ; index < srcStr.size ; index++) {
        append(srcStr.stringArr[index]);
    } 
}

MyString::MyString(const char* sctr2nd) {
    capacity = 8;
    size = 0 ;
    stringArr = new char[capacity];

    while(sctr2nd[size] != '\0') {
        append(sctr2nd[size]);
    }
}

MyString::~MyString() {
    delete[] stringArr;
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
    if(pos < size) {
        return stringArr[pos];
    } 
}

char& MyString::operator[](int pos) {
    if(pos < size) {
        return stringArr[pos];
    } 

    //return '\0';
}

const char& MyString::operator[](int pos) const {
    if(pos < size) {
        return stringArr[pos];
    } 

    return '\0';
}

int MyString::find(const char* cSubStr, int pos) {
    pos = pos < 0 ? 0 : pos;

    int index = 0;

    while(cSubStr[index] != '\0') {
        if(stringArr[pos + index] == cSubStr[index]) {
            index++;
        } else {
            pos == pos + index ? pos++ : pos = pos + index; 
            index = 0;
        }

        if(pos == size - 1) {
            return -1;
        }
    }

    return pos;
}

int MyString::find(const MyString& subStr, int pos) {
    pos = pos < 0 ? 0 : pos;

    int index = 0;

    while(subStr.stringArr[index] != '\0') {
        if(stringArr[pos + index] == subStr.stringArr[index]) {
            index++;
        } else {
            pos == pos + index ? pos++ : pos = pos + index; 
            index = 0;
        }

        if(pos == size - 1) {
            return -1;
        }
    }

    return pos;
}

MyString MyString::substr(int pos, int len) {
    MyString resultMyString = MyString();

    for(int index = pos ; index < pos + len ; index++) {
        if(stringArr[index] == '\0') {
            return resultMyString;
        } else {
            resultMyString.append(stringArr[index]);
        }
    }
}

const MyString MyString::operator+(const char* cstr2nd) {
    MyString resultMyString = MyString();

    for(int index = 0 ; index < size ; index++) {
        resultMyString.append(stringArr[index]);
    }

    int index = 0;

    while(cstr2nd[index] != '\0') {
        resultMyString.append(cstr2nd[index]);
        index++;
    }

    return resultMyString;
}

const MyString MyString::operator+(const MyString& str2nd) {
    MyString resultMyString = MyString();

    for(int index = 0 ; index < size ; index++) {
        resultMyString.append(stringArr[index]);
    }

    int index = 0;

    while(str2nd.stringArr[index] != '\0') {
        resultMyString.append(str2nd.stringArr[index]);
        index++;
    }

    return resultMyString;
}

bool MyString::operator==(const MyString& str) {
    if(size != str.size) {
        return false;
    } else {
        for(int index = 0 ; index < size ; index++) {
            if(stringArr[index] != str.stringArr[index]) {
                return false;
            }
        }
        return true;
    }
}

bool MyString::operator==(const char* cstr2nd) {
    int index = 0;
    while(cstr2nd[index] != '\0') {
        if (stringArr[index] != cstr2nd[index]) {
            return false;
        }
        index++;
    }
    return true;
}

bool MyString::operator!=(const MyString& str) {
    if(size != str.size) {
        return true;
    } else {
        for(int index = 0 ; index < size ; index++) {
            if(stringArr[index] != str.stringArr[index]) {
                return true;
            }
        }
        return false;
    }
}

bool MyString::operator!=(const char* cstr2nd) {
    int index = 0;
    while(cstr2nd[index] != '\0') {
        if (stringArr[index] != cstr2nd[index]) {
            return true;
        }
        index++;
    }
    return false;
}

MyString MyString::operator=(const MyString& srcStr) {
    if(&srcStr == this) {
        return *this;
    }

    delete[] stringArr;
    capacity = 8;
    size = 0;
    stringArr = new char[capacity];

    for(int index = 0 ; index < srcStr.size ; index++) {
        append(srcStr.stringArr[index]);
    }
    return *this;
}

MyString MyString::operator=(MyString&& srcStr) noexcept {
    if(this == &srcStr) {
        return *this;
    }

    delete[] stringArr;

    this->capacity = srcStr.capacity;
    this->size = srcStr.size;
    this->stringArr = srcStr.stringArr;

    srcStr.stringArr = nullptr;
    srcStr.size = 0;
    srcStr.capacity = 0;

    return *this;
}

MyString& MyString::operator=(const char* sctr2nd) {
    delete[] stringArr;
    this->capacity = 8;
    this->size = 0;
    stringArr = new char[capacity];

    while(sctr2nd[size] != '\0') {
        append(sctr2nd[size]);
    }

    stringArr[size] = '\0';

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
    bool isFirst = true;
    char inputChar;

    while (inputStream.get()) {
        if((32 < inputChar) && (inputChar < 127)) {
            str.append(inputChar);
            isFirst = false;
        } 
    }

    return inputStream;  
}

istream& getline(istream& inputStream, MyString& str, char delim) {
    char inputChar = inputStream.get();

    while (inputChar != delim) {
        str.append(inputChar);
    }

    return inputStream;
}

//'\0'처리 해줘야함