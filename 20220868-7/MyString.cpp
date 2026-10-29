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
    MyString result;
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i];
    }

    result += str2nd;
    return result;
}

MyString MyString::operator+(const MyString &other) const {
    MyString result(size + other.size + 1);
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i];
    }

    for (int i = 0; i < other.size; ++i) {
        result.data[i] = other.data[i];
    }

    result.data[size + other.size] = '\0';
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

MyString &MyString::operator+=(const char *v) {
    return append(v);
}

MyString &MyString::append(const MyString &v) {
    return append(v.getData());
}
MyString &MyString::append(const char *v) {
    const int prevSize = size;
    for (; v[size - prevSize] != '\0'; ++size) {
        if (size >= capacity - 1) {
            resize(capacity * 2);
        }
        data[size] = v[size - prevSize];
    }

    data[size] = '\0';
    return *this;
}

MyString &MyString::insert(int pos, const char *v) {
    int cnt = 0;
    pos = std::clamp(pos, 0, size);
    while (v[cnt] != '\0') {
        ++cnt;
    }

    const int newSize = size + cnt;
    resize((newSize + 33) >> 5 << 5);

    for (int i = 0; i < cnt; ++i) {
        data[pos + i] = v[i];
    }
    data[newSize] = '\0';
    size = newSize;
    return *this;
}

MyString &MyString::replace(const int pos, const int len, const MyString &v) {
    return replace(pos, len, v.data);
}

MyString &MyString::replace(const int pos, const int len, const char *v) {
    for (int i = 0; v[i] != '\0' && i < len && pos + i < size; ++i) {
        data[pos + i] = v[i];
    }
    return *this;
}

MyString &MyString::erase(const int pos, const int len) {
    const int oldSize = size;
    for (int i = pos + len; i < oldSize; ++i, --size) {
        data[pos + i] = data[pos + len + i];
    }
    data[size] = '\0';
    return *this;
}

int MyString::stoi(const MyString &str, const int pos, const int base) {
    int num = 0;
    for (int i = pos; i < str.size; ++i) {
        num *= base;
        const int add = charToInt(str[pos + i], base);
        if (add == -1) break;
        num += add;
    }
    return num;
}

int MyString::compare(const MyString &lhs, const MyString &rhs) {
    for (int i = 0; i < std::min(lhs.size, rhs.size); i++) {
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

    const int actualLen = std::min(endPos - pos, std::max(0, size - pos));
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

char MyString::at(const int pos) const {
    if (pos >= size) throw std::invalid_argument("invalid pos");
    return data[pos];
}

char MyString::operator[](const int pos) const {
    if (pos >= size) return '\0';
    return data[pos];
}

std::istream &MyString::getline(std::istream &in, MyString &str, const char delim) {
    const auto temp = new char[INITIAL_CAPACITY];
    char ch;
    int len;

    for (len = 0; (ch = in.get()) != delim && ch != EOF; ++len) {
        if (len >= str.capacity - 1) {
            str.resize(str.capacity * 2);
        }
        temp[len] = ch;
    }
    temp[len] = '\0';
    str = temp;
    delete[] temp;
    return in;
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

int MyString::charToInt(const char c, const int base) {
    assert(base >= 2 && base <= 36);
    int num = 0;
    if (c >= '0' && c <= '9') {
        num = c - '0';
    }
    if (c >= 'a' && c <= 'z') {
        num = c - 'a' + 10;
    }
    return num >= base ? -1 : num;
}


