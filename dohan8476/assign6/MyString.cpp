#include "MyString.h"
#include <iostream>

// =================================================================
// 기본 기능 난이도 하
// =================================================================

//1-1. default 생성자
MyString::MyString() {
    str[0] = '\0';
}
//1-2. 복사 생성자
MyString::MyString(const MyString& srcStr){
    int i = 0;
    for (; srcStr.str[i] != '\0' && i < CAPACITY; i++){
        str[i] = srcStr.str[i];
    }
    str[i] = '\0';
}
//1-3. 파괴자(구현)

MyString::~MyString() {

}

//1-4. 대입연산자
MyString &MyString::operator=(const MyString& srcStr) {
    if (this == &srcStr) {
        return *this;
    }
    *this = srcStr.str;

    return *this;
}

//1-5. 길이 반환
int MyString::length() const {
    return cstrlen(str);
}

//1-6. 문자열 비어있는지 검사
bool MyString::isEmpty() const{
    return str[0] == '\0';
}

//1-7. 전체 용량 리턴
int MyString::getCapacity() const {
    return CAPACITY;
}

//1-8. 문자열의 pos 위치에 있는 글자를 return 또는 참조
char MyString::at(const int pos) const {
    if (0 <= pos && pos < length()) {
        return str[pos];
    }
    return '\0';
}

char MyString::operator[](int pos) const {
    return this->at(pos);
}

//1-9 출력, 입력 관련

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
        //엔터, 공백 기준 짜르기
        if (c == '\n' || c == '\r' || c == ' ' || c == '\t'){
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

std::istream &getline(std::istream &is, MyString &str, const char delimiter) {
    char temp[16];
    char c;
    int i = 0;

    while (is.get(c)) {
        if (c == delimiter) {
            break;
        }
        //상수값 드러나는거 고치기
        if (i < 15) {
            temp[i++] = c;
        }
    }
    temp[i] = '\0';

    str = temp;

    return is;
}


// =================================================================
// 기본 기능 난이도 중
// =================================================================

//------------------------------------------------------------
//2-1. cstring 관련 생성자 및 대입 연산자
//------------------------------------------------------------
MyString::MyString(const char srcStr[]) {
    int i = 0;
    for (; srcStr[i] != '\0' && i < CAPACITY ; i++){
        str[i] = srcStr[i];
    }
    str[i] = '\0';
}

MyString &MyString::operator=(const char cstr2nd[]) {
    int i = 0;
    for (; cstr2nd[i] != '\0' && i < CAPACITY; i++) {
        str[i] = cstr2nd[i];
    }
    str[i] = '\0';
    return *this;
}
//------------------------------------------------------------
//2-2. 문자열 연결 연산자 오버로딩 (*this에 str2nd를 연결한 새 string 반환)
//------------------------------------------------------------
MyString MyString::operator+(const char str2nd[]) const {
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

MyString MyString::operator+(const MyString& str2nd) const {
    // 뒤에 .str을 붙히면 char 배열 -> 연산자를 만나서 위의 연산자 오버로딩 됨
    return *this + str2nd.str;
}
//------------------------------------------------------------
//2-3. 문자열 동등성을 비교하여 bool을 return하는 연산자 (==, !=)
//------------------------------------------------------------
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

bool MyString::operator==(const char cstr2nd[]) const {

}

bool MyString::operator!=(const MyString& srcStr) const {
    return !(*this == srcStr);
}

bool MyString::operator!=(const char cstr2nd[]) const {
    return !(*this == cstr2nd);
}

//------------------------------------------------------------
// 2-4. 부분문자열을 찾아 위치를 return하는 함수류(부분 문자열 없으면 -1)
//------------------------------------------------------------
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

        for (int j = 0; j < subLen && match; j++) {
            if (str[i + j] != subStr[j]) {
                match = false;
            }
        }
        if (match) {
            return i;
        }
    }
    return -1;
}

int MyString::find(const MyString& subStr, const int pos) const {
    return find(subStr.str, pos);
}

//------------------------------------------------------------
// 2-5. 부분문자열 추출 함수
//------------------------------------------------------------
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

// =================================================================
// 기본 기능 난이도 중상
// =================================================================

//------------------------------------------------------------
//3-1. 문자열 연결 연산자 함수류 (+=, append)
//------------------------------------------------------------
MyString &MyString::operator+=(const MyString &srcStr) {

}

MyString &MyString::operator+=(const char cstr2nd[]) {

}

MyString &MyString::append(const MyString &srcStr) {

}

MyString &MyString::append(const char cstr2nd[]) {

}

//------------------------------------------------------------
//3-2. 문자열 비교연산자류
//------------------------------------------------------------
bool MyString::operator<(const MyString& srcStr) const {

}

bool MyString::operator<(const char cstr2nd[]) const {

}

bool MyString::operator>(const MyString &srcStr) const {

}

bool MyString::operator>(const char cstr2nd[]) const {

}

bool MyString::operator<=(const MyString &srcStr) const {

}

bool MyString::operator<=(const char cstr2nd[]) const {

}

bool MyString::operator>=(const MyString &srcStr) const {

}

bool MyString::operator>=(const char cstr2nd[]) const {

}

//------------------------------------------------------------
//3-3. 문자열 수정 함수류(insert, replace, erase)
//------------------------------------------------------------
MyString &MyString::insert(int pos, const MyString &srcStr) {

}

MyString &MyString::insert(int pos, const char subStr[]) {

}

MyString &MyString::replace(int pos, int len, const MyString &subStr) {

}

MyString &MyString::replace(int pos, int len, const char subStr[]) {

}

MyString &MyString::erase(int pos, int len) {

}

//------------------------------------------------------------
// 3-4.  문자열을 숫자로 변환하는 stoi
//------------------------------------------------------------

int MyString::stoi(const MyString &str, int pos, int base) const {

}

// =================================================================
// PRIVATE
// =================================================================

int MyString::cstrlen(const char str[]) const {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}