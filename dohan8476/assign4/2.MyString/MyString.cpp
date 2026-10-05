#include "MyString.h"
#include <iostream>

MyString::MyString() {
    str[0] = '\0';
}

MyString::MyString(const MyString& srcStr){
    int i = 0;
    for (; srcStr.str[i] != '\0' && i < CAPACITY; i++){
        str[i] = srcStr.str[i];
    }
    str[i] = '\0';
}


MyString::MyString(const char srcStr[]) {
    int i = 0;
    for (; srcStr[i] != '\0' && i < CAPACITY ; i++){
        str[i] = srcStr[i];
    }
    str[i] = '\0';
}

int MyString::cstrlen(const char s[]) const {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

int MyString::length() const {
    return cstrlen(str);
}

bool MyString::empty() const{
    return str[0] == '\0';
}

char MyString::at(int pos) const {
    if (0 <= pos && pos < length()) {
        return str[pos];
    }
    return '\0';
}

char MyString::operator[](int pos) const {
    return this->at(pos);
}

int MyString::find(const char subStr[], int pos) const {
    if (pos < 0) {
        pos = 0;
    }

    int currentLen = length();
    int subLen = cstrlen(subStr);

    if (pos >= currentLen || subLen == 0 || currentLen < subLen) {
        return -1;
    }

    for (int i = pos; i <= currentLen - subLen; i++) {
        bool match = true;
        for (int j = 0; j < subLen; j++) {
            if (str[i + j] != subStr[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i;
        }
    }
    return -1;
}

int MyString::find(const MyString& subStr, int pos) const {
    return find(subStr.str, pos);
}

MyString MyString::substr(int pos, int len) const {
    MyString result;

    if (pos < 0) {
        pos = 0;
    }

    int currentLen = length();
    if (pos >= currentLen) {
        return result;
    }

    int i = 0;
    for(;i < len && pos + i < currentLen && i < CAPACITY; i++) {
        result.str[i] = str[pos + i];
    }
    result.str[i] = '\0';

    return result;
}

const MyString MyString::operator+(const char str2nd[]) const {
    MyString result;
    int i = 0;

    //앞배열 result로 복사
    for(;this->str[i] != '\0' && i < CAPACITY; i++) {
        result.str[i] = this->str[i];
    }

    //뒷 배열 result로 붙히기
    for (int j = 0 ;str2nd[j] != '\0' && i < CAPACITY; i++, j++) {
        result.str[i] = str2nd[j];
    }
    result.str[i] = '\0';

    return result;
}
const MyString MyString::operator+(const MyString& str2nd) const {
    // 뒤에 .str을 붙히면 char 배열 -> 연산자를 만나서 위의 연산자 오버로딩 됨
    return *this + str2nd.str;
}

MyString &MyString::operator=(const char str2nd[]) {
    int i = 0;
    for (; str2nd[i] != '\0' && i < CAPACITY; i++) {
        str[i] = str2nd[i];
    }
    str[i] = '\0';
    return *this;
}

MyString &MyString::operator=(const MyString& str2nd) {
    if (this == &str2nd) {
        return *this;
    }
    *this = str2nd.str;

    return *this;
}

bool MyString::operator==(const MyString& srcStr) const {
    int len = this->length();
    int len2 = srcStr.length();
    //길이 판별로 아래 문제 해소
    if (len != len2) {
        return false;
    }

    //this : abc / srcStr : abcd면 오류
    //this : abcde / srcStr : abc면 원하지 않는 메모리 터치
    for (int i = 0; i < len; i++) {
        if (this->str[i] != srcStr.str[i]) {
            return false;
        }
    }
    return true;
}
bool MyString::operator!=(const MyString& srcStr) const {
    return !(*this == srcStr);
}

std::ostream &operator<<(std::ostream &os, const MyString& str) {
    int len = str.length();
    for (int i = 0; i < len; i++) {
        os << str[i];
    }
    return os;
}

std::istream &operator>>(std::istream &is, MyString &str) {
    char temp[MyString::CAPACITY + 1];
    char c;
    int i = 0;

    while (is.get(c)) {

        //엔터치면 컷
        if (c == '\n' || c == '\r') {
            break;
        }

        if (i < MyString::CAPACITY) {
            temp[i++] = c;
        } else {
            //버퍼에 남기는 메소드
            is.putback(c);
            break;
        }
    }
    temp[i] = '\0';

    str = temp;
    return is;;
}

std::istream &getline(std::istream &is, MyString &str, char delimiter) {
    char temp[16];
    char c;
    int i = 0;

    while (is.get(c)) {
        if (c == delimiter) {
            break;
        }
        if (i < 15) {
            temp[i++] = c;
        }
    }
    temp[i] = '\0';

    str = temp;

    return is;
}
