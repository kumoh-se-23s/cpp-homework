#include "MyString.h"
#include <iostream>

// =================================================================
// 기본 기능 난이도 하
// =================================================================

//1-1. default 생성자
MyString::MyString() {
    str = new char[DEFAULT_CAPACITY];
    str[0] = '\0';
}
//1-2. 복사 생성자
MyString::MyString(const MyString& srcStr){
    size = srcStr.size;
    capacity = srcStr.capacity;
    str = new char[capacity];

    for (int i = 0; i < size ; i++) {
        str[i] = srcStr.str[i];
    }
    str[size] = '\0';
}

MyString::MyString(int newCapacity) {
    capacity = calcCapacity(newCapacity);
    str = new char[capacity];
    str[0] = '\0';
}

//1-3. 파괴자(구현)
MyString::~MyString() {
    delete[] str;
    str = nullptr;
}

//1-4. 대입연산자
MyString &MyString::operator=(const MyString& srcStr) {
    if (this == &srcStr) {
        return *this;
    }
    return assignHelper(srcStr.str, srcStr.size);
}

//1-5. 길이 반환
int MyString::length() const {
    return this->size;
}

//1-6. 문자열 비어있는지 검사
bool MyString::isEmpty() const{
    return size == 0;
}

//1-7. 전체 용량 리턴
int MyString::getCapacity() const {
    return this->capacity;
}

//1-8. 문자열의 pos 위치에 있는 글자를 return 또는 참조
char& MyString::at(int pos) const {
    if (0 <= pos && pos < length()) {
        return str[pos];
    }
    return str[size];
}

char& MyString::operator[](int pos) {
    if (0 <= pos && pos < length()) {
        return str[pos];
    }
    return str[size];
}

const char& MyString::operator[](int pos) const {
    if (0 <= pos && pos < length()) {
        return str[pos];
    }
    return str[size];
}

//1-9 출력, 입력 관련
static bool isWhitespace(char c) {
    return c == '\n' || c == '\r' || c == ' ' || c == '\t';
}

std::ostream &operator<<(std::ostream &os, const MyString& str) {
    for (int i = 0; i < str.length(); i++) {
        os << str[i];
    }
    return os;
}

std::istream &operator>>(std::istream &is, MyString &str) {
    char c;

    //앞쪽 공백 지우기
    while (is.get(c) && isWhitespace(c)) {}
    // "      "와 같은 입력에서 방어 코드
    //없으면 putback으로 공백 문자가 들어가 버그 발생
    if (!is) return is;
    // str 초기화
    str = "";
    //공백 다음 첫글자 넣어주기
    is.putback(c);

    while (is.get(c)) {
        //엔터, 공백 기준 짜르기
        if (isWhitespace(c)) {
            is.putback(c);
            break;
        }
        str.append(c);
    }
    return is;
}

std::istream &getline(std::istream &is, MyString &str, char delimiter) {
    // >> s1 >> s1
    str = "";
    char c;

    while (is.get(c)) {
        if (c == delimiter) {
            break;
        }
        str.append(c);
    }
    return is;
}

// =================================================================
// 기본 기능 난이도 중
// =================================================================

//------------------------------------------------------------
//2-1. cstring 관련 생성자 및 대입 연산자
//------------------------------------------------------------
MyString::MyString(const char srcStr[]) {
    size = cstrlen(srcStr);
    capacity = calcCapacity(size);
    str = new char[capacity];

    for (int i = 0; i < size ; i++){
        str[i] = srcStr[i];
    }
    str[size] = '\0';
}

MyString &MyString::operator=(const char cstr2nd[]) {
    if (this->str == cstr2nd) {
        return *this;
    }
    return assignHelper(cstr2nd, cstrlen(cstr2nd));
}

MyString &MyString::assignHelper(const char srcStr[], int strLen) {
    size = strLen;
    int newCap = calcCapacity(strLen);

    if (newCap != capacity) {
        delete[] str;
        capacity = newCap;
        str = new char[capacity];
    }

    for (int i = 0; i < size; i++) {
        str[i] = srcStr[i];
    }
    str[size] = '\0';
    return *this;
}

//------------------------------------------------------------
//2-2. 문자열 연결 연산자 오버로딩 (*this에 str2nd를 연결한 새 string 반환)
//------------------------------------------------------------
MyString MyString::operator+(const char str2nd[]) const {
    return plusHelper(str2nd, cstrlen(str2nd));
}

MyString MyString::operator+(const MyString& str2nd) const {
    return plusHelper(str2nd.str, str2nd.length());
}

// private method
MyString MyString::plusHelper(const char srcStr[], int strLen) const {
    MyString result(size + strLen);

    for (int i = 0; i < this->size; i++) {
        result.str[i] = this->str[i];
    }
    for (int i = 0; i < strLen; i++) {
        result.str[this->size + i] = srcStr[i];
    }
    result.size = this->size + strLen;
    result.str[result.size] = '\0';
    return result;
}

//------------------------------------------------------------
//2-3. 문자열 동등성을 비교하여 bool을 return하는 연산자 (==, !=)
//------------------------------------------------------------
bool MyString::operator==(const MyString& srcStr) const {
    return compareHelper(srcStr.str, srcStr.length());
}

bool MyString::operator==(const char cstr2nd[]) const {
    return compareHelper(cstr2nd, cstrlen(cstr2nd));
}

bool MyString::operator!=(const MyString& srcStr) const {
    return !(*this == srcStr);
}

bool MyString::operator!=(const char cstr2nd[]) const {
    return !(*this == cstr2nd);
}

//private
bool MyString::compareHelper(const char srcStr[], int strLen) const {
    if (this->length() != strLen) {
        return false;
    }
    for (int i = 0; i < this->length(); i++) {
        if (this->str[i] != srcStr[i]) {
            return false;
        }
    }
    return true;
}


//------------------------------------------------------------
// 2-4. 부분문자열을 찾아 위치를 return하는 함수류(부분 문자열 없으면 -1)
//------------------------------------------------------------
int MyString::find(const MyString& subStr, int pos) const {
    return findHelper(subStr.str, subStr.length(), pos);
}

int MyString::find(const char subStr[], int pos) const {
    return findHelper(subStr, cstrlen(subStr), pos);
}

//private sector
int MyString::findHelper(const char subStr[], int subLen, int pos) const {
    if (pos < 0) {
        pos = 0;
    }

    if (pos >= size || subLen == 0 || size < subLen) {
        return -1;
    }

    for (int i = pos; i <= size - subLen; i++) {
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

//------------------------------------------------------------
// 2-5. 부분문자열 추출 함수
//------------------------------------------------------------
MyString MyString::substr(int pos, int len) const {
    if (size == 0 || len <= 0 || pos < -size || pos >= size) {
        return {};
    }

    if (pos < 0) {
        int endPos = size + pos;
        int startPos = endPos - len + 1;

        if (startPos < 0) {
            startPos = 0;
            len = endPos + 1;
        }
        pos = startPos;
    }
    else {
        if (pos + len > size) {
            len = size - pos;
        }
    }

    MyString result(len);
    for (int i = 0; i < len; i++) {
        result.str[i] = str[pos + i];
    }
    result.str[len] = '\0';

    result.size = len;

    return result;
}

// =================================================================
// 기본 기능 난이도 중상
// =================================================================

//------------------------------------------------------------
//3-1. 문자열 연결 연산자 함수류 (+=, append)
//------------------------------------------------------------
MyString &MyString::operator+=(const MyString &srcStr) {
    return append(srcStr);
}

MyString &MyString::operator+=(const char cstr2nd[]) {
    return append(cstr2nd);
}

MyString &MyString::append(const MyString &srcStr) {
    if (srcStr.size <= 0) return *this;
    return replaceHelper(size, 0, srcStr.size, srcStr.str);
}

MyString &MyString::append(const char cstr2nd[]) {
    int len = cstrlen(cstr2nd);
    if (len <= 0) return *this;
    return replaceHelper(size, 0, len, cstr2nd);
}

MyString& MyString::append(char c) {
    if (size + 1>= capacity) {
        resize(size + 1);
    }
    str[size++] = c;
    str[size] = '\0';
    return *this;
}

// private method

//------------------------------------------------------------
//3-2. 문자열 비교연산자류
//------------------------------------------------------------
bool MyString::operator<(const MyString& srcStr) const {
    return compareTo(srcStr.str, srcStr.length()) < 0;
}

bool MyString::operator<(const char cstr2nd[]) const {
    return compareTo(cstr2nd, cstrlen(cstr2nd)) < 0;
}

bool MyString::operator>(const MyString &srcStr) const {
    return compareTo(srcStr.str, srcStr.length()) > 0;
}

bool MyString::operator>(const char cstr2nd[]) const {
    return compareTo(cstr2nd, cstrlen(cstr2nd)) > 0;
}

bool MyString::operator<=(const MyString &srcStr) const {
    return compareTo(srcStr.str, srcStr.length()) <= 0;
}

bool MyString::operator<=(const char cstr2nd[]) const {
    return compareTo(cstr2nd, cstrlen(cstr2nd)) <= 0;
}

bool MyString::operator>=(const MyString &srcStr) const {
    return compareTo(srcStr.str, srcStr.length()) >= 0;
}

bool MyString::operator>=(const char cstr2nd[]) const {
    return compareTo(cstr2nd, cstrlen(cstr2nd)) >= 0;
}

// private
int MyString::compareTo(const char srcStr[], int strLen) const {
    const int minLen = (size < strLen) ? size : strLen;
    for (int i = 0; i < minLen; i++) {
        if (str[i] != srcStr[i]) {
            return (str[i] > srcStr[i]) ? 1 : -1;
        }
    }
    if (size == strLen) return 0;
    // this : abcd / src : abc 면 this가 더 길다고 판단
    return (size > strLen) ? 1 : -1;
}

//------------------------------------------------------------
//3-3. 문자열 수정 함수류(insert, replace, erase)
//------------------------------------------------------------
MyString &MyString::insert(int pos, const MyString &srcStr) {
    return replaceHelper(pos, 0, srcStr.size, srcStr.str);
}

MyString &MyString::insert(int pos, const char subStr[]) {
    return replaceHelper(pos, 0, cstrlen(subStr), subStr);
}

MyString &MyString::replace(int pos, int len, const MyString &subStr) {
    return replaceHelper(pos, len, subStr.size, subStr.str);
}

MyString &MyString::replace(int pos, int len, const char subStr[]) {
    return replaceHelper(pos, len, cstrlen(subStr), subStr);
}

MyString &MyString::replaceHelper(int pos, int len, int strlen, const char srcStr[]) {
    pos = pos < 0 ? 0 : pos > size ? size : pos;

    if (len < 0 || pos + len > size) {
        len = size - pos;
    }

    int newSize = size + strlen - len;
    //need resize
    if (newSize >= capacity) {
        capacity = calcCapacity(newSize);
        char* temp = new char[capacity];

        for (int i = 0; i < pos; i++) {
            temp[i] = this->str[i];
        }
        for (int i = 0; i < strlen; i++) {
            temp[pos + i] = srcStr[i];
        }
        for (int i = pos + len; i < size; i++) {
            temp[i - len + strlen] = this->str[i];
        }
        delete[] str;
        str = temp;
    }
    else {
        char * temp = nullptr;
        // srcStr이 str[0~size]을 가리키는 경우
        if (str <= srcStr && srcStr <= str + size) {
            temp = new char[strlen];

            for (int i = 0; i < strlen; i++) {
                temp[i] = srcStr[i];
            }

            srcStr = temp;
        }

        if (len < strlen) {
            int shift = strlen - len;
            for (int i = size - 1; i >= pos + len; i--) {
                this->str[i + shift] = this->str[i];
            }
        }
        else if (len > strlen) {
            int shift = len - strlen;
            for (int i = pos + len; i < size; i++) {
                this->str[i - shift] = this->str[i];
            }
        }

        for (int i = 0; i < strlen; i++) {
            str[pos + i] = srcStr[i];
        }

        delete[] temp;
    }
    size = newSize;
    str[size] = '\0';
    return *this;
}

MyString &MyString::erase(int pos, int len) {
    return replaceHelper(pos, len, 0, "");
}

//------------------------------------------------------------
// 3-4.  문자열을 숫자로 변환하는 stoi(일반 함수)
//------------------------------------------------------------
static bool isDigit(const char c) {
    return '0' <= c && c <= '9';
}

static char toLower(const char c) {
    if ('A' <= c && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}

static bool isAlpha(char c) {
    c = toLower(c);
    return 'a' <= c && c <= 'z';
}

static int baseConverter(char c) {
    c = toLower(c);

    if (isDigit(c)) {
        return c - '0';
    }
    if (isAlpha(c)) {
        return c - 'a' + 10;
    }
    return -1;
}
//next isComvertible?
static bool isConvertible(const char c, int base) {
    const int val = baseConverter(c);
    return (0 <= val && val < base);
}

// isdigit, isAllDigit, baseConverter,,,, maybe more need to implement,,nn
int stoi(const MyString &str, int pos, int base) {
    if (base < 2 || base > 36) {
        return 0;
    }

    pos = pos < 0 ? 0 : pos > str.length() ? str.length() : pos;

    int res = 0;
    for (int i = pos; isConvertible(str[i], base); i++) {
        res *= base;
        res += baseConverter(str[i]);
    }

    return res;
}

//private 영역
int MyString::cstrlen(const char str[]) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void MyString::resize(int newCapacity) {
    capacity = calcCapacity(newCapacity);
    char* newStr = new char[capacity];

    for (int i = 0; i < size; i++) {
        newStr[i] = str[i];
    }
    newStr[size] = '\0';

    delete[] str;
    str = newStr;
}

int MyString::calcCapacity(int targetSize) const{
    int newCap = DEFAULT_CAPACITY;
    while (newCap <= targetSize) {
        newCap *= 2;
    }
    return newCap;
}


