#include <iostream>
#include "MyString.h"

//생성자---------------------------
MyString::MyString() {

}

MyString::MyString(const MyString &srcStr) {
    *this = srcStr;
}

//연산자 오버로딩---------------------
//대입
MyString MyString::operator=(const MyString &srcStr) const {
    MyString resultStr;
    for (int idx = 0; idx < srcStr.usingSize; idx++) {
        resultStr.str[idx] = srcStr.str[idx];
    }
    resultStr.usingSize = usingSize;
    resultStr.str[usingSize + 1] = '\0';
    return resultStr;
}
MyString &MyString::operator=(const char str2nd[]) const {
    MyString resultStr;
    int idx = 0; //usingSize 위해 따로 빼둠
    for (; idx < MAX_SIZE && str2nd[idx] != '\0'; idx++) {
        resultStr.str[idx] = str2nd[idx];
    }
    resultStr.usingSize = idx;
    resultStr.str[usingSize + 1] = '\0';
    return resultStr;
}

//+
const MyString MyString::operator+(const MyString &str2nd) const {
    MyString resultStr = *this;
    for (int idx = 0; idx + usingSize < MAX_SIZE && idx < str2nd.usingSize; idx++) {
        resultStr.str[usingSize + idx] = str2nd.str[idx];
    }
    resultStr.usingSize += str2nd.usingSize;
    resultStr.str[usingSize + 1] = '\0';
    return resultStr;
}
const MyString MyString::operator+(const char str2nd[]) const {
    MyString resultStr = *this;
    int idx = 0;
    for (; idx + usingSize < MAX_SIZE && str2nd[idx] != '\0'; idx++) {
        resultStr.str[usingSize + idx] = str2nd[idx];
    }
    resultStr.usingSize += idx;
    resultStr.str[usingSize + 1] = '\0';
    return resultStr;
}

//==
bool MyString::operator==(const MyString &str) const {
    if (usingSize != str.usingSize) //길이 다르면 바로 false
        return false;
    for (int idx = 0; idx < usingSize; idx++) {
        if (str[idx] != str.str[idx])
            return false;
    }
    return true;
}

//!=
bool MyString::operator!=(const MyString &str) const {
    return !(*this == str);
}

//[]
char MyString::operator[](int pos) const {
    if (pos < usingSize)
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
    char input[MyString::MAX_SIZE + 1];

    int idx = 0;
    for (; idx < MyString::MAX_SIZE; idx++) {
        input[idx] = cin.get();
        if (input[idx] == ' ') //공백 들어오면 그만 받고 탈출, 버퍼 유지
            break;
    }
    input[idx + 1] = '\0';
    str = input;
    return in;
}

istream& getline(istream& in, MyString& str, char delim) {
    char input[MyString::MAX_SIZE + 1];

    int idx = 0;
    for (; idx < MyString::MAX_SIZE; idx++) {
        input[idx] = cin.get();
        if (input[idx] == delim)
            break;
    }
    cin.ignore(1000, '\n');

    input[idx + 1] = '\0';
    str = input;
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
    if (pos < usingSize)
        return str[pos];
    return '\0';
}

int MyString::find(const MyString &subStr, int pos) const {
    if (subStr.usingSize == 0)
        return -1;
    for (int idx = 0; idx + pos < usingSize; idx++) {
        //첫 글자 같으면 substr로 범위 비교
        if (str[idx + pos] == subStr[idx]) {
            if (substr(idx + pos, subStr.usingSize) == subStr)
                return idx;
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
