
#pragma once
#include <cassert>
#include <iostream>
#include <compare>
#include <climits>


class MyString{
    char* data = nullptr;
    int size = 0;
    int capacity = 0;
    static constexpr int INITIAL_CAPACITY = 8;

public:

    explicit MyString(int initialCapacity = INITIAL_CAPACITY);
    MyString(const char *str);

    ~MyString();

    MyString (const MyString &other);
    MyString &operator=(const char str2nd[]);
    MyString &operator=(const MyString &other);
    MyString(MyString &&other) noexcept;
    MyString &operator=(MyString &&other) noexcept;

    MyString operator+(const char str2nd[]) const;
    MyString operator+(const MyString &other) const;

    bool operator==(const MyString &str) const;
    bool operator!=(const MyString &str) const;

    void append(char ch);


    [[nodiscard]] int find(const MyString &subStr, int pos = 0) const;
    [[nodiscard]] int find(const char *subStr, int pos = 0) const;

    MyString &operator+=(const MyString &v);

    MyString &operator+=(const char* v);

    MyString &append(const MyString &v, int len = INT_MAX);

    MyString &append(const char *v, int len = INT_MAX);

    MyString &insert(int pos, const MyString &v, int len = INT_MAX);

    MyString &insert(int pos, char v);

    MyString &insert(int pos, const char *v, int len = INT_MAX);

    MyString &replace(int pos, int len, const MyString &v);

    MyString &replace(int pos, int len, const char *v);

    MyString &erase(int pos, int len);

    template<typename T> requires std::is_arithmetic_v<T>
    static MyString toString(T num);

    std::partial_ordering operator<=>(const MyString &other) const {
        return compare(*this , other, std::min(size, other.size)) <=> 0;
    }

    [[nodiscard]] int compare(const MyString &v, int len = INT_MAX) const;
    [[nodiscard]] static int compare(const MyString &lhs, const MyString &rhs, int len = INT_MAX);

    [[nodiscard]] MyString substr(int pos) const;

    [[nodiscard]] MyString substr(int pos, int len) const;

    [[nodiscard]] int length() const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] char &at(int pos) const;
    [[nodiscard]] char &operator[](int pos);
    [[nodiscard]] const char &operator[](int pos) const;

    [[nodiscard]] const char * getData() const{
        return data;
    }

    [[nodiscard]] char * getData(){
        return data;
    }

    [[nodiscard]] int getCapacity() const {
        return capacity;
    }

    void resize(int newCapacity);

    static bool isWhitespace(char ch) {
        return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == EOF;
    }
private:
    MyString &replace(int pos, int len, const char *v, int vSize);
    MyString &insertReserve(int pos, int cnt);

};

template<typename T> requires std::is_arithmetic_v<T>
MyString MyString::toString(T num) {
    if (num == 0) return "0";

    MyString result;
    MyString tmp;
    while (num != 0) {
        tmp.append(num % 10 + '0');
        num /= 10;
    }
    for (int i = tmp.length() - 1; i >= 0; --i) {
        result.append(tmp.at(i));
    }
    return result;
}

int strlen(const char *c);

int charToInt(char c, int base);

int stoi(const MyString &str, int pos = 0, int base = 10);

std::istream &getline(std::istream &in, MyString &str, char delim = '\n');

std::ostream &operator<<(std::ostream &out, const MyString &string);

std::istream &operator>>(std::istream &in, MyString &str);

