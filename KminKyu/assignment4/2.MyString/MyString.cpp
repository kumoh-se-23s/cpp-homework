#include<iostream>
#include"MyString.h"

using namespace std;

MyString::MyString() {
    this->charArray[0] = '\0';
    this->size = 0;
    this->capacity = MAX_STR_SIZE - 1;
}

MyString::MyString(const MyString& srcStr) {
    this->capacity = MAX_STR_SIZE - 1;
    setString(srcStr.charArray);
}

MyString::MyString(const char str2nd[]) {
    this->capacity = MAX_STR_SIZE - 1;
    setString(str2nd);
}

MyString& MyString::operator=(const MyString& srcStr) {
    if (this == &srcStr){
        return *this;
    }
    setString(srcStr.charArray);
    return *this;
}

MyString& MyString::operator=(const char str2nd[]) {
    setString(str2nd);
    return *this;
}

int MyString::length() const {
    return size;
}

bool MyString::empty() const {
    return size == 0;
}

char MyString::at(int pos) const {
    if (0 <= pos && pos < size) {
        return charArray[pos];
    }
    return '\0';
}

char MyString::operator[](int pos) const {
    return at(pos);
}

const MyString MyString::operator+(const char str2nd[]) const {
    MyString result = MyString(charArray);
    result.append(str2nd);
    return result;
}

const MyString MyString::operator+(const MyString& str2nd) const {
    return *this + str2nd.charArray;
}

bool MyString::operator==(const MyString& str) const {
    return !(*this != str);
}

bool MyString::operator!=(const MyString& str) const {
    if (str.size != this->size) {
        return true;
    }
    for (int index = 0; charArray[index] != '\0'; ++index) {
        if (charArray[index] != str.charArray[index]) {
            return true;
        }
    }
    return false;
}

int MyString::find(const char subStr[], int pos) const {
    int subLength = 0;
    while (subStr[subLength] != '\0') ++subLength;
    return rabinKarp(subStr, pos, subLength);
}

int MyString::find(const MyString& subStr, int pos) const {
    return rabinKarp(subStr.charArray, pos, subStr.size);
}

MyString MyString::substr(int pos, int len) const {
    MyString result = MyString();
    pos = pos < 0 ? 0 : pos;

    if (pos >= size){
        return result;
    }


    for (int index = 0; index < len && charArray[index + pos] != '\0'; ++index) {
        result.append(charArray[index + pos]);
    }

    return result;
}

int MyString::getCapacity() const {
    return capacity;
}

void MyString::append(char appendChr) {
    if (size < capacity) {
        charArray[size] = appendChr;
        charArray[++size] = '\0';
    }
}

void MyString::append(const char appendStr[]) {
    for (int index = 0; appendStr[index] != '\0' && size < capacity; ++index) {
        charArray[size++] = appendStr[index];
    }
    charArray[size] = '\0';
}

void MyString::clear() {
    size = 0;
    charArray[0] = '\0';
}

void MyString::setString(const char str2nd[]) {
    for (size = 0; str2nd[size] != '\0' && size < capacity; ++size) {
        charArray[size] = str2nd[size];
    }
    charArray[size] = '\0';
}

int MyString::rabinKarp(const char subStr[], int pos, int subStrLength) const {
    pos = pos < 0 ? 0 : pos;
    if (subStrLength == 0 || subStrLength + pos > size) {
        return -1;
    }
    constexpr int HASH_BASE = 257, HASH_MOD = 1e9 + 7;
    long long int findHash = 0, resultHash = 0, highestPower = 1; //HASH_BASSE ^ (m-1)을 저장할 변수
    int endPos = size - subStrLength + 1;

    for (int index = 1; index < subStrLength; ++index) { //슬라이딩 할때마다 뺄 값 계산
        highestPower = highestPower * HASH_BASE % HASH_MOD;
    }
    for (int index = 0; index < subStrLength; ++index) { //첫 해시 값 계산 -> index -1이 불가능하므로 따로 계산
        findHash = (findHash * HASH_BASE + subStr[index]) % HASH_MOD;
        resultHash = (resultHash * HASH_BASE + charArray[pos + index]) % HASH_MOD;
    }
    for (int index = pos; index < endPos; ++index) {
        if (index != pos) { //해시 값 계산
            resultHash = (resultHash - charArray[index - 1] * highestPower % HASH_MOD + HASH_MOD) % HASH_MOD;
            resultHash = (resultHash * HASH_BASE + charArray[index + subStrLength - 1]) % HASH_MOD;
        }
        if (findHash == resultHash && isSame(charArray, subStr, index, index + subStrLength - 1)) {
            //해시 값이 같으면 실제로 같은지 확인 -> 이중 해시 쓰면 확률이 낮아짐
            return index;
        }
    }
    return -1;

}

bool MyString::isSame(const char str1st[], const char str2nd[], int start, int end) {
    for (int index = start; index <= end; ++index) {
        if (str1st[index] != str2nd[index - start]) {
            return false;
        }
    }
    return true;
}

ostream& operator<<(ostream& os, const MyString& str){
    for (int index = 0; index < str.length(); ++index) {
        os << str.at(index);
    }
    return os;
}

istream& operator>>(istream& is, MyString& str) {
    str.clear();
    char input;

    while (is.get(input) && input != '\n' && input != ' ' && input != '\r') {
        str.append(input);
        if (str.length() == str.getCapacity()) {
            return is;
        }
    }

    return is;
}

istream& getline(istream& is, MyString& str, char delim) {
    str.clear();
    char input;
    while (is.get(input) && input != delim) {
        str.append(input);
    }

    return is;
}
