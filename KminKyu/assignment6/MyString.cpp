#include <iostream>
#include "MyString.h"

using namespace std;

static char FAILED_CHAR = '\0';

MyString::MyString() {
    this->charArray = new char[DEFAULT_CAPACITY];
    this->size = 0;
    this->charArray[size] = '\0';
    this->capacity = DEFAULT_CAPACITY;
}

MyString::MyString(const MyString& srcStr) {
    this->charArray = new char[srcStr.capacity];
    this->capacity = srcStr.capacity;

    setString(srcStr.charArray);
}

MyString::MyString(const char *cstr2nd) {
    this->charArray = new char[DEFAULT_CAPACITY];
    this->capacity = DEFAULT_CAPACITY;
    setString(cstr2nd);
}

MyString::MyString(MyString &&srcStr) {
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
    if (this == &srcStr){
        return *this;
    }
    setString(srcStr.charArray);
    return *this;
}

MyString& MyString::operator =(MyString &&srcStr) {
    if (this == &srcStr) {
        return *this;
    }
    delete[] this->charArray;

    this->size = srcStr.size;
    this->capacity = srcStr.capacity;
    this->charArray = srcStr.charArray;

    srcStr.capacity = 0;
    srcStr.size = 0;
    srcStr.charArray = nullptr;

    return *this;
}

MyString& MyString::operator =(const char *str2nd) {
    setString(str2nd);
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
    return FAILED_CHAR;
}

char& MyString::operator [](int pos) {
    return at(pos);
}

const char& MyString::operator [](int pos) const {
    if (0 <= pos && pos < size) {
        return charArray[pos];
    }
    return charArray[size];
}

MyString MyString::operator +(const char *cstr2nd) const {
    MyString result = MyString(charArray);
    result.append(cstr2nd);
    return result;
}

MyString MyString::operator +(const MyString& str2nd) const {
    return *this + str2nd.charArray;
}

bool MyString::operator ==(const MyString& str) const {
    if (str.size != this->size) {
        return false;
    }
    for (int index = 0; charArray[index] != '\0' && str[index] != '\0'; ++index) {
        if (charArray[index] != str.charArray[index]) {
            return false;
        }
    }
    return true;
}

bool MyString::operator !=(const MyString& str) const {
    return !(*this == str);
}

bool MyString::operator ==(const char *cstr2nd) const {
    for (int index = 0; charArray[index] != '\0' && cstr2nd[index] != '\0'; ++index) {
        if (charArray[index] != cstr2nd[index]) {
            return false;
        }
    }
    return true;
}

bool MyString::operator !=(const char *cstr2nd) const {
    return !(*this == cstr2nd);
}

int MyString::find(const char *subStr, int pos) const {
    if (pos < 0) {
        pos = 0;
    } else if (pos >= size) {
        pos = size -1;
    }
    int subLength;
    for (subLength = 0; subStr[subLength] != '\0'; ++subLength) {}
    return boyerMoore(subStr, pos, subLength);
}

int MyString::find(const MyString& subStr, int pos) const {
    if (pos < 0) {
        pos = 0;
    } else if (pos >= size) {
        pos = size -1;
    }
    return boyerMoore(subStr.charArray, pos, subStr.size);
}

MyString MyString::substr(int pos, int len) const {
    MyString result = MyString();

    if (pos < 0) {
        pos = size + pos % size;

    } else if (pos >= size) {
        pos = size -1;
    }

    if (pos + len >= size) {
        len = size - pos;
    }

    for (int index = 0; index < len && charArray[index + pos] != '\0'; ++index) {
        result.append(charArray[index + pos]);
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

    for (int index = 0; cstr2nd[index] != '\0'; ++index) {
        if (size >= capacity) {
            resize();
        }
        charArray[size++] = cstr2nd[index];
    }
    charArray[size] = '\0';

    return *this;
}

MyString& MyString::append(const MyString &str) {
    return this->append(str.charArray);
}


bool MyString::operator <(const MyString &str) const {
    return (compare(str.charArray) == -1);
}

bool MyString::operator >(const MyString &str) const {
    return (compare(str.charArray) == 1);
}

bool MyString::operator <=(const MyString &str) const {
    int result = compare(str.charArray);
    return (result == -1 || result == 0);
}

bool MyString::operator >=(const MyString &str) const {
    int result = compare(str.charArray);
    return (result == 1 || result == 0);
}

bool MyString::operator <(const char *cstr2nd) const {
    return (compare(cstr2nd) == -1);
}

bool MyString::operator >(const char *cstr2nd) const {
    return (compare(cstr2nd) == 1);
}

bool MyString::operator <=(const char *cstr2nd) const {
    int result = compare(cstr2nd);
    return (result == -1 || result == 0);
}

bool MyString::operator >=(const char *cstr2nd) const {
    int result = compare(cstr2nd);
    return (result == 1 || result == 0);
}

int MyString::compare(const char *str2nd) const {
    int index;
    for (index = 0; this->charArray[index] != '\0' && str2nd[index] != '\0'; ++index) {
        if (this->charArray[index] < str2nd[index]) {
            return -1;
        } else if (this->charArray[index] > str2nd[index]) {
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
    if (pos < 0) {
        pos = 0;
    } else if (pos >= size) {
        pos = size -1;
    }
    MyString resultString = MyString();
    for (int index = 0; index < size; ++index) {
        if (index == pos) {
            for (int subStrIndex = 0; subStr[subStrIndex] != '\0'; ++subStrIndex) {
                resultString.append(subStr[subStrIndex]);
            }
        }
        resultString.append(charArray[index]);
    }
    *this = resultString;
    return *this;
}

MyString& MyString::insert(int pos, const MyString &subStr) {
    return insert(pos, subStr.charArray);
}

MyString& MyString::replace(int pos, int len, const char *subStr) {
    if (pos < 0) {
        pos = 0;
    } else if (pos >= size) {
        pos = size -1;
    }

    if (pos + len >= size) {
        len = size - pos;
    }
    MyString resultString = MyString();
    for (int index = 0; index < size; ++index) {
        if (index == pos) {
            for (; index < pos + len && subStr[index - pos] != '\0'; ++index) {
                resultString.append(subStr[index - pos]);
            }
        }
        resultString.append(charArray[index]);
    }

    *this = resultString;
    return *this;
}

MyString& MyString::replace(int pos, int len, const MyString &subStr) {
    return replace(pos, len, subStr.charArray);
}

MyString& MyString::erase(int pos, int cnt) {
    if (pos < 0) {
        pos = 0;
    } else if (pos >= size) {
        pos = size -1;
    }

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
    charArray[0] = '\0';
}

void MyString::resize() {
    this->capacity = (capacity == 0) ? 8 : capacity * 2;

    char *result = new char[capacity];

    for (int index = 0; index < size; ++index) {
        result[index] = charArray[index];
    }
    delete[] charArray;
    charArray = result;
}

//생성자, 대입연산자에서만 사용하는 깊은복사
void MyString::setString(const char *str2nd) {
    for (size = 0; str2nd[size] != '\0'; ++size) {
        if (size >= this->capacity) {
            resize();
        }
        charArray[size] = str2nd[size];
    }
    charArray[size] = '\0';
}

int MyString::boyerMoore(const char subStr[], int pos, int subStrLength) const {
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
        os << str[index];
    }
    return os;
}

istream& operator>>(istream& is, MyString& str) {
    str.clear();
    char input;
    is.get(input);

    while (!is.fail() && isspace(input)) {
        is.get(input);
    }
    is.putback(input);

    while (is.get(input) && !isspace(input) && (str.length() < str.getCapacity())) {
        str.append(input);
    }
    is.putback(input);
    return is;
}

istream& getline(istream& is, MyString& str, char delim) {
    str.clear();
    char input;
    while (is.get(input) && input != delim) {
        if (str.length() < str.getCapacity()) {
            str.append(input);
        }
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
