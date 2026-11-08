#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <utility>
#include "MyString.h"

using namespace std;

MyString::MyString() {
    copyString("", DEFAULT_CAPACITY);
}

MyString::MyString(const MyString& srcStr) {
    copyString(srcStr.charArray, srcStr.getCapacity());
}

MyString::MyString(const char *cstr2nd) {
    int cstr2ndSize = getCharArraySize(cstr2nd);
    copyString(cstr2nd, calcInitialCapacity(cstr2ndSize));

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

MyString& MyString::operator =(MyString &&srcStr) noexcept {
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
        int str2ndSize = getCharArraySize(str2nd);
        copyString(str2nd, calcInitialCapacity(str2ndSize));
    }
    return *this;
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
    return result.append(cstr2nd);
}

MyString MyString::operator +(const MyString& str2nd) const {
    MyString result = MyString(charArray);
    return result.append(str2nd.charArray, str2nd.size);
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

int MyString::find(const char *subStr, int pos) const {
    if (subStr == nullptr) {
        return -1;
    }

    return rabinKarp(subStr, positionNormalize(pos), getCharArraySize(subStr));
}

int MyString::find(const MyString& subStr, int pos) const {

    return rabinKarp(subStr.charArray, positionNormalize(pos), subStr.size);
}

MyString MyString::substr(int pos, int len) const {
    MyString result = MyString();
    if (pos < 0) {
        int temp = max(pos - len + size + 1, 0);
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
    return append(cstr2nd);
}

MyString& MyString::operator +=(const MyString &str) {
    return append(str.charArray, str.size);
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
    return this->append(cstr2nd, getCharArraySize(cstr2nd));
}

MyString & MyString::append(const char *cstr2nd, int cstr2ndSize) {

    char *copyStr = nullptr;

    if (cstr2nd == charArray) {
        copyStr =copyCharArray(cstr2nd, cstr2ndSize);
        cstr2nd = copyStr;
    }

    if (cstr2ndSize + size >= capacity) {
        resize(calcInitialCapacity(cstr2ndSize + size));
    }

    for (int index = 0; index < cstr2ndSize; ++index) {
        charArray[size++] = cstr2nd[index];
    }

    charArray[size] = '\0';

    if (copyStr != nullptr) {
        delete[] copyStr;
    }

    return *this;
}
MyString& MyString::append(const MyString &str) {
    return this->append(str.charArray, str.size);
}

bool MyString::operator <(const MyString &str) const {
    return compare(str.charArray) == -1;
}

bool MyString::operator >(const MyString &str) const {
    return compare(str.charArray) == 1;
}

bool MyString::operator <=(const MyString &str) const {
    return !(*this > str);
}

bool MyString::operator >=(const MyString &str) const {
    return !(*this < str);
}

bool MyString::operator <(const char *cstr2nd) const {
    return compare(cstr2nd) == -1;
}

bool MyString::operator >(const char *cstr2nd) const {
    return compare(cstr2nd) == 1;
}

bool MyString::operator <=(const char *cstr2nd) const {
    return !(*this > cstr2nd);
}

bool MyString::operator >=(const char *cstr2nd) const {
    return !(*this < cstr2nd);
}

MyString& MyString::insert(int pos, const char *subStr) {
    if (subStr == nullptr) {
        return *this;
    }
    return insert(pos, subStr, getCharArraySize(subStr));

}
MyString & MyString::insert(int pos, const char *subStr, int subStrSize) {
    char *copyStr = nullptr;

    if (subStr == charArray) {
        copyStr = copyCharArray(subStr, subStrSize);
        subStr = copyStr;
    }

    pos = positionNormalize(pos);

    if (size + subStrSize >= capacity) {
        resize(calcInitialCapacity(size + subStrSize));
    }
    for (int index = size - 1; index >= pos; --index) {
        charArray[index + subStrSize] = charArray[index];
    }

    for (int index = pos; index < pos + subStrSize; ++index) {
        charArray[index] = subStr[index - pos];
    }

    size += subStrSize;
    charArray[size] = '\0';

    if (copyStr != nullptr) {
        delete[] copyStr;
    }

    return *this;
}

MyString& MyString::insert(int pos, const MyString &subStr) {
    return insert(pos, subStr.charArray);
}

MyString& MyString::replace(int pos, int len, const char *subStr) {
    if (subStr == nullptr) {
        return *this;
    }
    return replace(pos, len, subStr, getCharArraySize(subStr));
}

MyString& MyString::replace(int pos, int len, const MyString &subStr) {
    return replace(pos, len, subStr.charArray);
}
MyString & MyString::replace(int pos, int len, const char *subStr, int subStrSize) {
    char *copyStr = nullptr;

    if (subStr == charArray) {
        copyStr = copyCharArray(subStr, subStrSize);
        subStr = copyStr;
    }

    pos = positionNormalize(pos);

    len = lengthNormalize(pos, len);

    if (size - len + subStrSize >= capacity) {
        resize(calcInitialCapacity(size - len + subStrSize));
    }

    for (int index = pos + len; index < size; ++index) {
        charArray[index - len] = charArray[index];
    }

    for (int index = size - len; index >= pos; --index) {
        charArray[index + subStrSize] = charArray[index];
    }

    for (int index = pos; index < pos + subStrSize; ++index) {
        charArray[index] = subStr[index - pos];
    }

    size = size - len + subStrSize;
    charArray[size] = '\0';

    if (copyStr != nullptr) {
        delete[] copyStr;
    }

    return *this;
}

MyString& MyString::erase(int pos, int cnt) {
    pos = positionNormalize(pos);

    cnt = lengthNormalize(pos, cnt);

    for (int index = pos; index < size; ++index) {
        charArray[index] = charArray[index + cnt];
    }
    size -= cnt;
    charArray[size] = '\0';
    return *this;

}

void MyString::clear() {
    size = 0;
    if (charArray != nullptr) {
        charArray[0] = '\0';
    }

}

void MyString::resize() {
    if (capacity == 0) {
        resize(DEFAULT_CAPACITY);
    } else {
        resize(capacity * 2);
    }
}

void MyString::resize(int reCapacity) {
    this->capacity = reCapacity;
    char *result = new char[reCapacity + 1];

    for (int index = 0; index < size; ++index) {
        result[index] = charArray[index];
    }
    result[size] = '\0';

    delete[] charArray;
    charArray = result;
}

int MyString::getCharArraySize(const char *str2nd) {
    int cnt;
    for (cnt = 0; str2nd[cnt] != '\0'; ++cnt) {}
    return cnt;
}

char *MyString::copyCharArray(const char *baseStr, int baseStrSize) {
    char *resultStr = new char[baseStrSize + 1];
    for (int index = 0; index < baseStrSize; ++index) {
        resultStr[index] = baseStr[index];
    }
    resultStr[baseStrSize] = '\0';
    return resultStr;
}


//생성자, 대입연산자에서만 사용하는 깊은복사 및 size, capacity 설정
void MyString::copyString(const char *copyStr, int copyStrCapacity) {
    char *result;
    if (this->capacity < copyStrCapacity) {
        result = new char[max(copyStrCapacity, 0) + 1];
    } else {
        result = this->charArray;
    }

    for (size = 0; copyStr[size] != '\0'; ++size) {
        result[size] = copyStr[size];
    }

    result[size] = '\0';

    if (result != this->charArray) {
        delete[] charArray;
        this->charArray = result;
    }
    this->capacity = copyStrCapacity;
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

int MyString::rabinKarp(const char *subStr, int pos, int subStrSize) const {

    if (subStrSize == 0) {
        return pos;
    }

    if (subStrSize + pos > size) {
        return -1;
    }

    constexpr int HASH_BASE = 257, HASH_MOD = 1e9 + 7;

    long long int findHash = 0, resultHash = 0, highestPower = 1; //HASH_BASE ^ (m-1)을 저장할 변수

    int endPos = size - subStrSize + 1;

    for (int index = 1; index < subStrSize; ++index) { //슬라이딩 할때마다 뺄 값 계산
        highestPower = highestPower * HASH_BASE % HASH_MOD;
    }
    for (int index = 0; index < subStrSize; ++index) { //첫 해시 값 계산 -> index -1이 불가능하므로 따로 계산
        findHash = (findHash * HASH_BASE + static_cast<unsigned char>(subStr[index])) % HASH_MOD;
        resultHash = (resultHash * HASH_BASE + static_cast<unsigned char>(charArray[pos + index])) % HASH_MOD;
    }
    for (int index = pos; index < endPos; ++index) {
        if (index != pos) { //해시 값 계산
            resultHash = (resultHash - static_cast<unsigned char>(charArray[index - 1]) * highestPower % HASH_MOD + HASH_MOD) % HASH_MOD;
            resultHash = (resultHash * HASH_BASE + static_cast<unsigned char>(charArray[index + subStrSize - 1])) % HASH_MOD;
        }
        if (findHash == resultHash && isSame(charArray, subStr, index, index + subStrSize - 1)) {
            //해시 값이 같으면 실제로 같은지 확인 -> 이중 해시 쓰면 확률이 낮아짐
            return index;
        }
    }
    return -1;
}

int MyString::positionNormalize(int pos) const {
    if (pos < 0) {
        pos = 0;
    } else if (pos > size) {
        pos = size;
    }
    return pos;
}

int MyString::lengthNormalize(int pos, int len) const {
    if (len > size - pos || len < 0) {
        return size - pos;
    }
    return len;
}


bool MyString::isSame(const char *str1st, const char *str2nd, int start, int end) {
    for (int index = start; index <= end; ++index) {
        if (str1st[index] != str2nd[index - start]) {
            return false;
        }
    }
    return true;
}

int MyString::calcInitialCapacity(int arraySize) {
    if (arraySize <= DEFAULT_CAPACITY) {
        return DEFAULT_CAPACITY;
    }
    int result;
    for (result = DEFAULT_CAPACITY; result <= arraySize; result *= 2) {}
    return result;
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
    //
    //앞 공백 제거 코드 -> 실제 string과 다른 작동이므로 테스트에서 버그가 나서 잠깐보류
    //
    // while (input != EOF && isspace(input)) {
    //     input = is.get();
    // }
    // if (input == EOF) {
    //     return is;
    // }

    while (input != EOF && static_cast<char>(input) != delim) {
        str.append(static_cast<char>(input));
        input = is.get();
    }
    if (input == EOF && !str.isEmpty()) {
        is.clear();
    }
    return is;
}

int stoi(const MyString &str, int pos, int base) {
    constexpr int MAX_BASE = 36;
    constexpr int MIN_BASE = 2;

    int result = 0;
    if (pos < 0) {
        pos = 0;
    } else if (pos > str.length()) {
        pos = std::max(str.length(), pos);
    }
    if (base < MIN_BASE) {
        base = MIN_BASE;
    } else if (base > MAX_BASE) {
        base = MAX_BASE;
    }

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
        return (alphabet - 'a') + ('9' - '0' + 1);
    } else if  ('A' <= alphabet && alphabet <= 'Z') {
        return (alphabet - 'A') + ('9' - '0' + 1);
    }
    return -1;
}
