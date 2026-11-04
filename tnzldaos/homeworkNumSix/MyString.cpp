#include "MyString.h"

//신경 써야 할 부분 
//1.capacnity
//2.size
//3.배열
//4.'\0'

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

void MyString::appendChar(char inputChar) {
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

    if(srcStr.size == 0) {
        stringArr[size] = '\0';
    } else {
        for(int index = 0 ; index < srcStr.size ; index++) {
            appendChar(srcStr.stringArr[index]);
        } 
    }
}

MyString::MyString(const char* sctr2nd) {
    capacity = 8;
    size = 0 ;
    stringArr = new char[capacity];

    while(sctr2nd[size] != '\0') {
        appendChar(sctr2nd[size]);
    }

    if(size == 0) {
        stringArr[size] = '\0';
    }
}

MyString::~MyString() {
    delete[] stringArr;
}

//----------------------------------------------------------------------

void MyString::clear() {
    delete[] stringArr;

    stringArr = new char[capacity];
}

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
    if(pos >= 0 && pos < size) {
        return stringArr[pos];
    } 

    char result = '\0';
    return result;
}

char& MyString::operator[](int pos) {
    if(pos >= 0 && pos < size) {
        return stringArr[pos];
    } 

    char result = '\0';
    return result;
}

const char& MyString::operator[](int pos) const {
    if(pos >= 0 && pos < size) {
        return stringArr[pos];
    } 

    char result = '\0';
    return result;
}

int MyString::find(const char* cSubStr, int pos) const {
    pos = pos < 0 ? 0 : (pos > size - 1 ? size - 1 : pos);

    int index = 0;

    while(cSubStr[index] != '\0' && stringArr[index] != '\0') {
        if(stringArr[pos + index] == cSubStr[index]) {
            index++;
        } else {
            pos++; 
            index = 0;
        }

        if(pos > size - 1) {
            return -1;
        }
    }
    return pos;
}

int MyString::find(const MyString& subStr, int pos) const {
    pos = pos < 0 ? 0 : (pos > size - 1 ? size - 1 : pos);

    int index = 0;
    while(subStr.stringArr[index] != '\0' && stringArr[index] != '\0') {
        if(stringArr[pos + index] == subStr.stringArr[index]) {
            index++;
        } else {
            pos++;
            index = 0;
        }

        if(pos > size - 1) {
            return -1;
        }
    }
    return pos;
}

MyString MyString::substr(int pos, int len) const {
    MyString resultMyString = MyString();

    if(pos > size || -pos > size) {
        resultMyString.append("");
        return resultMyString;
    }

    if(pos >= 0) {
        len = len + pos > size ? size - pos : len; 
        
        for(int index = pos ; index < pos + len ; index++) {
            resultMyString.appendChar(stringArr[index]);       
        }
    } else {
        pos = pos + size;
        int start = pos - len + 1 < 0 ? 0 : pos - len + 1;

        for(int index = start ; index <= pos ; index++) {
            resultMyString.appendChar(stringArr[index]);       
        }
    }

    return resultMyString;
}

MyString& MyString::insert(int pos, const char* subStr) {
    char* resultCharArr = new char[capacity];

    pos = pos < 0 ? pos + size :(pos > size ? size : pos);

    int newSize = 0;

    for(int index = 0 ; index < pos ; index++) {
        resultCharArr[index] = stringArr[index];
        newSize++;
    }

    int inputCharArrSize = 0;
    while(subStr[inputCharArrSize] != '\0') {
        resultCharArr[pos + inputCharArrSize] = subStr[inputCharArrSize];
        inputCharArrSize++;
        newSize++;
    }

    for(int index = pos ; index < size ; index++) {
        resultCharArr[pos + inputCharArrSize] = stringArr[index];
        newSize++;
    }

    delete[] stringArr;
    stringArr = resultCharArr;
    size = newSize;

    return *this;
}

MyString& MyString::insert(int pos, const MyString& subStr) {
    char* resultCharArr = new char[capacity];

    pos = pos < 0 ? pos + size :(pos > size ? size : pos);

    for(int index = 0 ; index < pos ; index++) {
        resultCharArr[index] = stringArr[index];
    }

    for(int index = 0 ; index < size ; index++) {
        resultCharArr[pos + index] = subStr.stringArr[index];
    }

    for(int index = pos ; index < size ; index++) {
        resultCharArr[pos + subStr.size] = stringArr[index];
    }

    delete[] stringArr;
    stringArr = resultCharArr;

    return *this;
}

MyString& MyString::replace(int pos, int len, const char* subStr) {
    

}

MyString& MyString::replace(int pos, int len, const MyString& subStr) {

}

MyString& MyString::erase(int pos, int cnt) {
    if(pos >= size || -pos > size) {
        delete[] stringArr;
        capacity = 8;
        size = 0;
        stringArr = new char[capacity];
        stringArr[0] = '\0';

        return *this;
    }

    char* resultCharArr = new char[capacity];
    resultCharArr[0] = '\0';

    int resultIndex = 0;
    if(pos >= 0) {

        for(int index = 0 ; index < pos ; index++) {
            resultCharArr[resultIndex] = stringArr[index];
            resultIndex++;
        }

        for(int index = pos + cnt ; index < size ; index++) {
            resultCharArr[resultIndex] = stringArr[index];
            resultIndex++;
        }
    } else {
        pos = pos + size;

        for(int index = 0 ; index <= pos - cnt ; index++) {
            resultCharArr[resultIndex] = stringArr[index];
            resultIndex++;
        } 

        for(int index = pos + 1 ; index < size ; index++) {
            resultCharArr[resultIndex] = stringArr[index];
            resultIndex++;
        }
    }

    delete[] stringArr;
    stringArr = resultCharArr;
    size = resultIndex;

    return *this;
}

int MyString::stoi(const MyString& str, int pos, int base) {

}


const MyString MyString::operator+(const char* cstr2nd) const {
    MyString resultMyString = MyString();

    for(int index = 0 ; index < size ; index++) {
        resultMyString.appendChar(stringArr[index]);
    }

    int index = 0;

    while(cstr2nd[index] != '\0') {
        resultMyString.appendChar(cstr2nd[index]);
        index++;
    }

    return resultMyString;
}

const MyString MyString::operator+(const MyString& str2nd) const {
    MyString resultMyString = MyString();

    for(int index = 0 ; index < size ; index++) {
        resultMyString.appendChar(stringArr[index]);
    }

    int index = 0;

    while(str2nd.stringArr[index] != '\0') {
        resultMyString.appendChar(str2nd.stringArr[index]);
        index++;
    }

    return resultMyString;
}

bool MyString::operator==(const MyString& str) const {
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

bool MyString::operator==(const char* cstr2nd) const {
    int index = 0;

    while(cstr2nd[index] != '\0' && stringArr[index] != '\0') {
        if (stringArr[index] != cstr2nd[index]) {
            return false;
        }
        index++;
    }

    if(cstr2nd[index] == '\0' && stringArr[index] == '\0') {
        return true;
    } else {
        return false;
    }
}

bool MyString::operator!=(const MyString& str) const {
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

bool MyString::operator!=(const char* cstr2nd) const {
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
        appendChar(srcStr.stringArr[index]);
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
        appendChar(sctr2nd[size]);
    }

    stringArr[size] = '\0';

    return *this;
}

//---------------------------------------------------------------------

MyString& MyString::operator+=(const char* cstr2nd) {
    int index = 0;
    while(cstr2nd[index] != '\0') {
        appendChar(cstr2nd[index]);
        index++;
    }

    return *this;
}

MyString& MyString::operator+=(const MyString& str) {
    int nowSize = str.size;
    for(int index = 0 ; index < nowSize ; index++) {
        appendChar(str.stringArr[index]);
    }

    return *this;
}

MyString& MyString::append(const char* cstr2nd) {
    int index = 0;
    while(cstr2nd[index] != '\0') {
        appendChar(cstr2nd[index]);
        index++;
    }

    return *this;
}

MyString& MyString::append(const MyString& str) {
    int nowSize = str.size;

    for(int index = 0 ; index < nowSize ; index++) {
        appendChar(str.stringArr[index]);
    }

    return *this;
}

bool MyString::operator<(const MyString& str) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && str.stringArr[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = str.stringArr[index];

        if(lhsChar > rhsChar) {
            return false;
        } else if (lhsChar < rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] == '\0' && str.stringArr[index] != '\0') {
        return true;
    } else {
        return false;
    }
}

bool MyString::operator<=(const MyString& str) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && str.stringArr[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = str.stringArr[index];

        if(lhsChar > rhsChar) {
            return false;
        } else if (lhsChar < rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] == '\0' && str.stringArr[index] != '\0') {
        return true;
    } else if (stringArr[index] != '\0' && str.stringArr[index] == '\0') {
        return false;
    } else {
        return true;
    }
}

bool MyString::operator>(const MyString& str) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && str.stringArr[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = str.stringArr[index];

        if(lhsChar < rhsChar) {
            return false;
        } else if (lhsChar > rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] != '\0' && str.stringArr[index] == '\0') {
        return true;
    } else {
        return false;
    }
}

bool MyString::operator>=(const MyString& str) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && str.stringArr[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = str.stringArr[index];

        if(lhsChar < rhsChar) {
            return false;
        } else if (lhsChar > rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] != '\0' && str.stringArr[index] == '\0') {
        return true;
    } else if (stringArr[index] == '\0' && str.stringArr[index] != '\0') {
        return false;
    } else {
        return true;
    }
}

bool MyString::operator<(const char* cstr2nd) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && cstr2nd[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = cstr2nd[index];

        if(lhsChar > rhsChar) {
            return false;
        } else if (lhsChar < rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] == '\0' && cstr2nd[index] != '\0') {
        return true;
    } else {
        return false;
    }
}

bool MyString::operator<=(const char* cstr2nd) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && cstr2nd[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = cstr2nd[index];

        if(lhsChar > rhsChar) {
            return false;
        } else if (lhsChar < rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] == '\0' && cstr2nd[index] != '\0') {
        return true;
    } else if (stringArr[index] != '\0' && cstr2nd[index] == '\0') {
        return false;
    } else {
        return true;
    }
}

bool MyString::operator>(const char* cstr2nd) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && cstr2nd[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = cstr2nd[index];

        if(lhsChar < rhsChar) {
            return false;
        } else if (lhsChar > rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] != '\0' && cstr2nd[index] == '\0') {
        return true;
    } else {
        return false;
    }
}

bool MyString::operator>=(const char* cstr2nd) const {
    int index = 0 ;
    while(stringArr[index] != '\0' && cstr2nd[index] != '\0') {
        char lhsChar = stringArr[index];
        char rhsChar = cstr2nd[index];

        if(lhsChar < rhsChar) {
            return false;
        } else if (lhsChar > rhsChar) {
            return true;
        }

        index++;
    }

    if(stringArr[index] != '\0' && cstr2nd[index] == '\0') {
        return true;
    } else if (stringArr[index] == '\0' && cstr2nd[index] != '\0') {
        return false;
    } else {
        return true;
    }
}

//--------------------------------------------------------------------

ostream& operator<<(ostream& outputStream, const MyString& str) {
    for(int index = 0 ; index < str.length() ; index++) {
        outputStream << str[index];
    }

    return outputStream; 
}

istream& operator>>(istream& inputStream, MyString& str) {
    str.clear();

    bool isFirst = true;
    bool isRun = true;
    
    while (isRun) {
        char inputChar = inputStream.get();
        
        if((inputChar == ' ' || inputChar == '\t' || inputChar == '\n') && !isFirst) {
            isRun = false;
        } else if ((inputChar == ' ' || inputChar == '\t' || inputChar == '\n') && isFirst) {
            
        } else {
            str.appendChar(inputChar);
            isFirst = false;
        }
    }
    
    return inputStream;  
}

istream& getline(istream& inputStream, MyString& str, char delim) {
    str.clear();

    char inputChar = inputStream.get();

    while (inputChar != delim && inputChar != '\n') {
        str.appendChar(inputChar);

        inputChar = inputStream.get();
    }
    
    return inputStream;  
}