#include "MyString.h"

#include <algorithm>
#include <cassert>

MyString::MyString(const int initialCapacity) {
    data = new char[initialCapacity];
    capacity = initialCapacity;
}

MyString::MyString(const char *str) {
    data = new char[INITIAL_CAPACITY];
    capacity = INITIAL_CAPACITY;
    append(str);
}


MyString::~MyString() {
    delete[] data;
}

MyString::MyString(const MyString &other) {
    size = other.size;
    data = new char[other.capacity];
    capacity = other.capacity;

    for (int i = 0; i < other.size; ++i) {
        data[i] = other[i];
    }
    data[size] = '\0';
};

MyString &MyString::operator=(const MyString &other) {
    if (this == &other) return *this;

    if (this->size != other.size) {
        delete[] data;
        data = new char[other.capacity];
    }

    size = other.size;
    capacity = other.capacity;

    for (int i = 0; i < other.size; ++i) {
        data[i] = other[i];
    }
    data[size] = '\0';
    return *this;
}

MyString::MyString(MyString &&other) noexcept {
    std::swap(size, other.size);
    std::swap(data, other.data);
    std::swap(capacity, other.capacity);
}

MyString &MyString::operator=(MyString &&other) noexcept {
    if (this == &other) return *this;

    std::swap(size, other.size);
    std::swap(data, other.data);
    std::swap(capacity, other.capacity);
    return *this;
}


MyString &MyString::operator=(const char str2nd[]) {
    for (size = 0; str2nd[size] != '\0'; ++size) {
        if (size >= capacity - 1) {
            resize(capacity * 2);
        }
        data[size] = str2nd[size];
    }
    data[size] = '\0';
    return *this;
};

MyString MyString::operator+(const char str2nd[]) const {
    MyString result(size);
    result = data;
    result += str2nd;
    return result;
}

MyString MyString::operator+(const MyString &other) const {
    MyString result(size + other.size + 1);
    result = data;
    result += other.data;
    return result;
}

bool MyString::operator==(const MyString &str) const {
    if (size != str.size) return false;
    for (int i = 0; i < size; ++i) {
        if (data[i] != str.data[i]) return false;
    }
    return true;
};

bool MyString::operator!=(const MyString &str) const {
    return !(*this == str);
}

void MyString::append(const char ch) {
    if (size >= capacity - 1) {
        resize(capacity * 2);
    }

    data[size] = ch;
    data[++size] = '\0';
}

int MyString::find(const MyString &subStr, int pos) const {
    pos = std::max(0, pos);
    for (int i = pos; i < size; ++i) {
        int idx = i;
        for (int j = 0; j < subStr.size; ++j) {
            if (i + j >= size) return -1;
            if (data[i + j] != subStr[j]) idx = -1;
        }
        if (idx >= 0) return idx;
    }
    return -1;
};

int MyString::find(const char *subStr, int pos) const {
    pos = std::max(0, pos);
    for (int i = pos; i < size; ++i) {
        int idx = i;
        for (int j = 0; subStr[j] != '\0'; ++j) {
            if (i + j >= size) return -1;
            if (data[i + j] != subStr[j]) idx = -1;
        }
        if (idx >= 0) return idx;
    }
    return -1;
}

MyString &MyString::operator+=(const MyString &v) {
    return append(v);
}

MyString &MyString::operator+=(const char *v) {
    return append(v);
}

MyString &MyString::append(const MyString &v, const int len) {
    return append(v.getData(), len);
}
MyString &MyString::append(const char *v, const int len) {
    if (v == data) {
        const MyString temp = v;
        return append(temp.data, len);
    }
    const int prevSize = size;
    for (; v[size - prevSize] != '\0' && size - prevSize < len; ++size) {
        if (size >= capacity - 1) {
            resize(capacity * 2);
        }
        data[size] = v[size - prevSize];
    }

    data[size] = '\0';
    return *this;
}

MyString &MyString::insert(int pos, const MyString &v, const int len) {
    return insert(pos, v.data, len);
}

MyString &MyString::insert(int pos, const char v) {

    pos = std::clamp(pos, 0, size);
    const int newSize = size + 1;
    resize((newSize + 33) >> 5 << 5);
    for (int i = newSize; i > pos; --i) {
        data[i] = data[i - 1];
    }

    data[pos] = v;
    size = newSize;
    return *this;
}

MyString &MyString::insert(int pos, const char *v, const int len) {
    if (v == data) {
        const MyString temp = v;
        return insert(pos, temp.data, len);
    }


    const int cnt = std::min(strlen(v), len);
    insertReserve(pos, cnt);

    for (int i = 0; i < cnt; ++i) {
        data[pos + i] = v[i];
    }

    return *this;
}

MyString &MyString::replace(const int pos, const int len, const MyString &v) {
    return replace(pos, len, v.data, v.size);
}

MyString &MyString::replace(const int pos, const int len, const char *v) {
    if (v == data) {
        const MyString temp = v;
        return replace(pos, len, temp.data, temp.size);
    }

    return replace(pos, len, v, strlen(v));
}

MyString &MyString::erase(const int pos, int len) {
    const int oldSize = size;
    len = std::clamp(len, 0, size - pos);
    for (int i = pos + len; i < oldSize; ++i) {
        data[pos + i] = data[pos + len + i];
    }
    size -= len;
    data[size] = '\0';
    return *this;
}

int MyString::compare(const MyString &v, const int len) const {
    return compare(*this, v, len);
}

int MyString::compare(const MyString &lhs, const MyString &rhs, const int len) {
    for (int i = 0; i < std::min(len, std::min(lhs.size, rhs.size)); i++) {
        const int diff = lhs[i] - rhs[i];
        if (diff != 0) {
            return (diff > 0) - (diff < 0);
        }
    }
    return (lhs.size > rhs.size) - (lhs.size < rhs.size);
}

MyString MyString::substr(const int pos) const {
    return substr(pos, size - pos);
}

MyString MyString::substr(int pos, const int len) const {

    if (pos < 0) pos = size + pos - len + 1;
    const int endPos = std::max(0, pos + len);
    pos = std::max(0, pos);

    const int actualLen = std::max(0, std::min(endPos - pos, size - pos));
    const auto temp = new char[actualLen + 1];
    for (int i = 0; i < actualLen; ++i) {
        temp[i] = data[pos + i];
    }
    temp[actualLen] = '\0';
    MyString result = temp;
    delete[] temp;
    return result;
};

int MyString::length() const {
    return size;
};

bool MyString::isEmpty() const {
    return size == 0;
}

char &MyString::at(const int pos) const {
    if (pos >= size || pos < 0) return data[size];
    return data[pos];
}

char &MyString::operator[](const int pos) {
    if (pos >= size || pos < 0) return data[size];
    return data[pos];
}

const char &MyString::operator[](const int pos) const {
    if (pos >= size || pos < 0) return data[size];
    return data[pos];
}


void MyString::resize(const int newCapacity) {
    assert(newCapacity >= 0);
    const auto newPtr = new char[newCapacity];
    for (int i = 0; i < size; ++i) {
        newPtr[i] = data[i];
    }
    newPtr[size] = '\0';
    delete[] data;
    data = newPtr;
    capacity = newCapacity;
}

MyString & MyString::replace(const int pos, const int len, const char *v, const int vSize) {
    const int diff = vSize - len;
    if (diff > 0) {
        insertReserve(pos, diff);
    }else if (diff < 0) {
        erase(pos,-diff);
    }

    for (int i = 0; i < vSize; ++i) {
        data[pos + i] = v[i];
    }
    return *this;
}

MyString & MyString::insertReserve(int pos, int cnt) {
    pos = std::clamp(pos, 0, size);

    const int newSize = size + cnt;
    resize((newSize + 33) >> 5 << 5);

    for (int i = newSize; i > pos; --i) {
        data[i] = data[i - cnt];
    }

    size = newSize;
    return *this;
}


int strlen(const char *c) {
    int cnt = 0;
    while (c[cnt] != '\0') {
        ++cnt;
    }
    return cnt;
}

int charToInt(const char c, const int base) {
    assert(base >= 2 && base <= 36);
    int num = 0;
    if (c >= '0' && c <= '9') {
        num = c - '0';
    }
    if (c >= 'A' && c <= 'Z') {
        num = c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'z') {
        num = c - 'a' + 10;
    }
    return num >= base ? -1 : num;
}

int stoi(const MyString &str, const int pos, const int base) {
    int num = 0;
    for (int i = pos; i < str.length(); ++i) {
        num *= base;
        const int add = charToInt(str[pos + i], base);
        if (add == -1) break;
        num += add;
    }
    return num;
}

std::istream & getline(std::istream &in, MyString &str, const char delim) {
    char ch;
    MyString temp;
    while ((ch = in.get()) != delim && ch != EOF) {
        temp.append(ch);
    }
    str = temp;
    return in;
}

std::ostream & operator<<(std::ostream &out, const MyString &string) {
    out << string.getData();
    return out;
}

std::istream & operator>>(std::istream &in, MyString &str) {
    char ch = 0;
    str = "";
    while (str.isEmpty() || !MyString::isWhitespace(ch)) {
        ch = in.get();
        if (!MyString::isWhitespace(ch)) {
            str.append(ch);
        }
    }
    return in;
}


