#include <iostream>
#include "MyString.h"
#include <limits>

//생성자---------------------------
MyString::MyString() {
    str[usingSize] = '\0';
}

MyString::MyString(const MyString &srcStr) {
    *this = srcStr;
}

//연산자 오버로딩---------------------
//대입
MyString& MyString::operator=(const MyString &srcStr) {
    for (int idx = 0; idx < srcStr.usingSize; idx++) {
        str[idx] = srcStr.str[idx];
    }
    usingSize = srcStr.usingSize;
    str[usingSize] = '\0';
    return *this;
}
MyString& MyString::operator=(const char str2nd[]) {
    int idx = 0; //usingSize 위해 따로 빼둠
    for (; idx < MAX_SIZE && str2nd[idx] != '\0'; idx++) {
        str[idx] = str2nd[idx];
    }
    usingSize = idx;
    str[usingSize] = '\0';
    return *this;
}

//+
const MyString MyString::operator+(const MyString &str2nd) const {
    MyString resultStr = *this;
    int idx = 0;
    for (; idx + usingSize < MAX_SIZE && idx < str2nd.usingSize; idx++) {
        resultStr.str[usingSize + idx] = str2nd.str[idx];
    }
    resultStr.usingSize += idx;
    resultStr.str[resultStr.usingSize] = '\0';
    return resultStr;
}
const MyString MyString::operator+(const char str2nd[]) const {
    MyString operand;
    operand = str2nd;
    return *this + operand;
}

//==
bool MyString::operator==(const MyString &str2nd) const {
    if (usingSize != str2nd.usingSize) //길이 다르면 바로 false
        return false;
    for (int idx = 0; idx < usingSize; idx++) {
        if (str2nd[idx] != str2nd.str[idx])
            return false;
    }
    return true;
}

//!=
bool MyString::operator!=(const MyString &str2nd) const {
    return !(*this == str2nd);
}

//[]
char MyString::operator[](int pos) const {
    if (pos >= 0 && pos < usingSize)
        return str[pos];
    return '\0';
}

//출력
ostream& operator<<(ostream & out, const MyString &str) {
    for (int idx = 0; idx < str.length(); idx++) {
        out << str.at(idx);
    }
    return out;
}

istream& operator>>(istream& in, MyString& str) {
    char result[MyString::MAX_SIZE + 1];
    char input;
    in.get(input);

    int idx = 0;
    for (; idx < MyString::MAX_SIZE && input != ' '; in.get(input)) {
        result[idx] = input;
    }
    result[idx] = '\0';
    str = result;
    return in;
}

istream& getline(istream& in, MyString& str, char delim) {
    char result[MyString::MAX_SIZE + 1];
    char input;
    in.get(input);

    int idx = 0;
    for (; idx < MyString::MAX_SIZE && input != delim; idx++, in.get(input)) {
        result[idx] = input;
    }
    result[idx] = '\0';
    in.ignore(numeric_limits<streamsize>::max(), '\n');

    str = result;
    return in;
}

//기능------------------------------
int MyString::length() const {
    return usingSize;
}

bool MyString::empty() const {
    return usingSize == 0;
}

char MyString::at(int pos) const {
    if (pos >= 0 && pos < usingSize)
        return str[pos];
    return '\0';
}

int MyString::find(const MyString &subStr, int pos) const {
    if (pos < 0)
        pos = 0;
    if (subStr.usingSize == 0)
        return -1;
    for (int idx = 0; idx + pos < usingSize; idx++) {
        //첫 글자 같으면 substr로 범위 비교
        if (str[idx + pos] == subStr[0]) {
            if (substr(idx + pos, subStr.usingSize) == subStr)
                return idx + pos;
        }
    }
    return -1;
}

int MyString::find(const char subStr[], int pos) const {
    MyString target;
    target = subStr;
    return find(target, pos);
}

MyString MyString::substr(int pos, int len) const {
    //pos 처리
    if (pos < 0) //음수처리
        pos = 0;
    else if (pos > usingSize) //초과처리
        return MyString();

    //len 초과처리
    if (pos + len > usingSize)
        len = usingSize - pos;

   //동작
    MyString resultStr;
    for (int idx = 0; idx < len; idx++) {
        resultStr.str[idx] = str[idx + pos];
    }
    resultStr.usingSize = len;

    return resultStr;
}
