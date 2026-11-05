#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include "MyString.h"

using namespace std;

MyString::MyString() {
    copyString("", DEFAULT_CAPACITY);
}

MyString::MyString(const MyString& srcStr) {
    copyString(srcStr.charArray, srcStr.getCapacity());
}

MyString::MyString(const char *cstr2nd) {
    copyString(cstr2nd, growCapacity(cstr2nd));

}

MyString::MyString(MyString &&srcStr) noexcept {

    this->charArray = srcStr.charArray;
    this->size = srcStr.size;
    this->capacity = srcStr.capacity;

    srcStr.charArray = nullptr;
    srcStr.size = 0;
    srcStr.capacity = 0;
}

MyString::~MyString() {
    delete[] charArray;
}

MyString& MyString::operator =(const MyString& srcStr) {
    if (this != &srcStr){
        copyString(srcStr.charArray, srcStr.capacity);
    }
    return *this;
}

MyString& MyString::operator =(MyString &&srcStr) {
    if (this == &srcStr) {
        return *this;
    }

    if (charArray != nullptr) {
        delete[] this->charArray;
    }

    this->size = srcStr.size;
    this->capacity = srcStr.capacity;
    this->charArray = srcStr.charArray;

    srcStr.capacity = 0;
    srcStr.size = 0;
    srcStr.charArray = nullptr;

    return *this;
}

MyString& MyString::operator =(const char *str2nd) {
    if (charArray != str2nd) {
        copyString(str2nd, growCapacity(str2nd));
    }
    return *this;
}

int MyString::length() const {
    return size;
}

bool MyString::empty() const {
    return size == 0;
}

int MyString::getCapacity() const {
    return capacity;
}

char& MyString::at(int pos) {
    if (0 <= pos && pos < size) {
        return charArray[pos];
    }
    return charArray[size];
}

const char & MyString::at(int pos) const {
    if (0 <= pos && pos < size) {
        return charArray[pos];
    }
    return charArray[size];
}

char& MyString::operator [](int pos) {
    return at(pos);
}

const char& MyString::operator [](int pos) const {
    return at(pos);
}

MyString MyString::operator +(const char *cstr2nd) const {
    MyString result = MyString(charArray);
    result.append(cstr2nd);
    return result;
}

MyString MyString::operator +(const MyString& str2nd) const {
    return *this + str2nd.charArray;
}

int MyString::find(const char *subStr, int pos) const {
    if (subStr == nullptr) {
        return -1;
    }
    pos = positionNormalize(pos);

    int subLength;
    for (subLength = 0; subStr[subLength] != '\0'; ++subLength) {}
    return rabinKarp(subStr, pos, subLength);
}

int MyString::find(const MyString& subStr, int pos) const {
    pos = positionNormalize(pos);
    return rabinKarp(subStr.charArray, pos, subStr.size);
}

MyString MyString::substr(int pos, int len) const {
    MyString result = MyString();
    int temp = 0;
    if (pos < 0) {
        temp = max(pos - len + size + 1, 0);
        len =  (pos + size + 1) - temp;
        pos = temp;
    }
    if (pos < 0 || size <= pos || len < 0) {
        return result;
    }

    for (int index = pos; index < pos + len && index < size; ++index) {
        result.append(charArray[index]);
    }

    return result;
}

MyString& MyString::operator +=(const char *cstr2nd) {
    *this = *this + cstr2nd;

    return *this;
}

MyString& MyString::operator +=(const MyString &str) {
    *this = *this + str;

    return *this;
}

MyString& MyString::append(char appendChr) {
    if (size >= capacity) {
        resize();
    }

    charArray[size] = appendChr;
    charArray[++size] = '\0';

    return *this;
}

MyString &MyString::append(const char *cstr2nd) {
    if (cstr2nd == nullptr) {
        return *this;
    }

    char *appendStr = nullptr;
    if (cstr2nd == charArray) {
        appendStr = new char[size + 1];
        for (int index = 0; index <= size; ++index) {
            appendStr[index] = charArray[index];
        }
        cstr2nd = appendStr;
    }

    for (int index = 0; cstr2nd[index] != '\0'; ++index) {
        if (size >= capacity) {
            resize();
        }
        charArray[size++] = cstr2nd[index];
    }

    charArray[size] = '\0';

    //appendStr을 사용한경우(자기자신이 인자로 들어온 경우)
    if (appendStr != nullptr) {
        delete[] appendStr;
    }

    return *this;
}

MyString& MyString::append(const MyString &str) {
    return this->append(str.charArray);
}

bool MyString::operator ==(const MyString& str) const {
    return compare(str.charArray) == 0;
}

bool MyString::operator !=(const MyString& str) const {
    return !(*this == str);
}

bool MyString::operator ==(const char *cstr2nd) const {
    return compare(cstr2nd) == 0;
}

bool MyString::operator !=(const char *cstr2nd) const {
    return !(*this == cstr2nd);
}

bool MyString::operator <(const MyString &str) const {
    return compare(str.charArray) == -1;
}

bool MyString::operator >(const MyString &str) const {
    return compare(str.charArray) == 1;
}

bool MyString::operator <=(const MyString &str) const {
    int result = compare(str.charArray);
    return result == -1 || result == 0;
}

bool MyString::operator >=(const MyString &str) const {
    int result = compare(str.charArray);
    return result == 1 || result == 0;
}

bool MyString::operator <(const char *cstr2nd) const {
    return compare(cstr2nd) == -1;
}

bool MyString::operator >(const char *cstr2nd) const {
    return compare(cstr2nd) == 1;
}

bool MyString::operator <=(const char *cstr2nd) const {
    int result = compare(cstr2nd);
    return result == -1 || result == 0;
}

bool MyString::operator >=(const char *cstr2nd) const {
    int result = compare(cstr2nd);
    return result == 1 || result == 0;
}

int MyString::compare(const char *str2nd) const {
    if (str2nd == nullptr) {
        return 1;
    }

    int index;

    for (index = 0; this->charArray[index] != '\0' && str2nd[index] != '\0'; ++index) {
        unsigned char thisChar = this->charArray[index];
        unsigned char str2ndChar = str2nd[index];
        if (thisChar < str2ndChar) {
            return -1;
        } else if (thisChar > str2ndChar) {
            return 1;
        }
    }

    if (this->charArray[index] != '\0') {
        return 1;
    } else if (str2nd[index] != '\0') {
        return -1;
    } else {
        return 0;
    }
}


MyString& MyString::insert(int pos, const char *subStr) {
    if (subStr == nullptr) {
        return *this;
    }

    pos = positionNormalize(pos);

    MyString resultString = MyString();

    for (int index = 0; index < pos; ++index) {
        resultString.append(charArray[index]);
    }
    resultString.append(subStr);
    for (int index = pos; index < size; ++index) {
        resultString.append(charArray[index]);
    }
    *this = resultString;
    return *this;
}

MyString& MyString::insert(int pos, const MyString &subStr) {
    return insert(pos, subStr.charArray);
}

MyString& MyString::replace(int pos, int len, const char *subStr) {
    if (subStr == nullptr) {
        return *this;
    }

    pos = positionNormalize(pos);

    if (pos + len >= size) {
        len = size - pos;
    }
    MyString resultString = MyString();

    for (int index = 0; index < pos; ++index) {
        resultString.append(charArray[index]);
    }
    resultString.append(subStr);
    for (int index = pos + len; index < size; ++index) {
        resultString.append(charArray[index]);
    }

    *this = resultString;
    return *this;
}

MyString& MyString::replace(int pos, int len, const MyString &subStr) {
    return replace(pos, len, subStr.charArray);
}

MyString& MyString::erase(int pos, int cnt) {
    pos = positionNormalize(pos);

    if (pos + cnt >= size) {
        cnt = size - pos;
    }
    MyString resultString = MyString();
    for (int index = 0; index < size; ++index) {
        if (index < pos || pos + cnt <= index) {
            resultString.append(charArray[index]);
        }
    }
    *this = resultString;
    return *this;

}

void MyString::clear() {
    size = 0;
    if (charArray != nullptr) {
        charArray[0] = '\0';
    }

}

void MyString::resize() {
    this->capacity = (capacity == 0) ? DEFAULT_CAPACITY : capacity * 2;

    char *result = new char[capacity + 1];

    for (int index = 0; index < size; ++index) {
        result[index] = charArray[index];
    }
    result[size] = '\0';

    delete[] charArray;
    charArray = result;
}

//생성자, 대입연산자에서만 사용하는 깊은복사 및 size, capacity 설정
void MyString::copyString(const char *copyStr, int copyStrCapacity) {
    char *result = new char[copyStrCapacity + 1];

    for (size = 0; copyStr[size] != '\0'; ++size) {
        result[size] = copyStr[size];
    }
    result[size] = '\0';
    delete[] charArray;
    this->charArray = result;
    this->capacity = copyStrCapacity;
}

int MyString::rabinKarp(const char *subStr, int pos, int subStrLength) const {
    pos = pos < 0 ? 0 : pos;

    if (subStrLength == 0) {
        return pos;
    }

    if (subStrLength + pos > size) {
        return -1;
    }

    constexpr int HASH_BASE = 257, HASH_MOD = 1e9 + 7;

    long long int findHash = 0, resultHash = 0, highestPower = 1; //HASH_BASSE ^ (m-1)을 저장할 변수

    int endPos = size - subStrLength + 1;

    for (int index = 1; index < subStrLength; ++index) { //슬라이딩 할때마다 뺄 값 계산
        highestPower = highestPower * HASH_BASE % HASH_MOD;
    }
    for (int index = 0; index < subStrLength; ++index) { //첫 해시 값 계산 -> index -1이 불가능하므로 따로 계산
        findHash = (findHash * HASH_BASE + static_cast<unsigned char>(subStr[index])) % HASH_MOD;
        resultHash = (resultHash * HASH_BASE + static_cast<unsigned char>(charArray[pos + index])) % HASH_MOD;
    }
    for (int index = pos; index < endPos; ++index) {
        if (index != pos) { //해시 값 계산
            resultHash = (resultHash - static_cast<unsigned char>(charArray[index - 1]) * highestPower % HASH_MOD + HASH_MOD) % HASH_MOD;
            resultHash = (resultHash * HASH_BASE + static_cast<unsigned char>(charArray[index + subStrLength - 1])) % HASH_MOD;
        }
        if (findHash == resultHash && isSame(charArray, subStr, index, index + subStrLength - 1)) {
            //해시 값이 같으면 실제로 같은지 확인 -> 이중 해시 쓰면 확률이 낮아짐
            return index;
        }
    }
    return -1;
}

int MyString::positionNormalize(int pos) const {
    if (pos < 0) {
        pos = 0;
    } else if (pos >= size) {
        pos = std::max(size - 1, 0);
    }
    return pos;
}



bool MyString::isSame(const char *str1st, const char *str2nd, int start, int end) {
    for (int index = start; index <= end; ++index) {
        if (str1st[index] != str2nd[index - start]) {
            return false;
        }
    }
    return true;
}

int MyString::growCapacity(const char *cstr2nd) {
    int cstr2ndSize;
    for (cstr2ndSize = 0; cstr2nd[cstr2ndSize] != '\0'; ++cstr2ndSize) {}

    if (cstr2ndSize < DEFAULT_CAPACITY) {
        return DEFAULT_CAPACITY;
    }
    return (cstr2ndSize + DEFAULT_CAPACITY - 1) / DEFAULT_CAPACITY * DEFAULT_CAPACITY;

}


ostream& operator<<(ostream& os, const MyString& str){
    for (int index = 0; index < str.length(); ++index) {
        os << str[index];
    }
    return os;
}

istream& operator>>(istream& is, MyString& str) {
    str.clear();

    int input = is.get();

    while (input != EOF && isspace(input)) {
        input = is.get();
    }
    if (input == EOF) {
        return is;
    }
    while (input != EOF && !isspace(input)) {
        str.append(static_cast<char>(input));
        input = is.get();
    }

    if (input == EOF) {
        is.clear();
    } else {
        is.putback(static_cast<char>(input));
    }

    return is;
}

istream& getline(istream& is, MyString& str, char delim) {
    str.clear();
    int input = is.get();
    while (input != EOF && static_cast<char>(input) != delim) {
        str.append(static_cast<char>(input));
        input = is.get();
    }
    if (input == EOF && !str.empty()) {
        is.clear();
    }
    return is;
}

int stoi(const MyString &str, int pos, int base) {
    int result = 0;
    for (int index = pos; index < str.length(); ++index) {

        int changeValue = changeAlphabetToInt(str[index]);

        if (changeValue != -1 && changeValue < base) {
            result *= base;
            result += changeValue;

        } else {
            break;
        }
    }
    return result;
}
int changeAlphabetToInt(char alphabet) {
    if ('0' <= alphabet && alphabet <= '9') {
        return alphabet - '0';
    } else if ('a' <= alphabet && alphabet <= 'z') {
        return (alphabet - 'a') + 10;
    }
    return -1;
}
