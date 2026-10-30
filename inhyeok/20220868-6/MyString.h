
#pragma once
#include <iostream>
#include <compare>


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

    MyString &operator+=(const char* v);

    MyString &append(const char *v);

    MyString &insert(int pos, const char *v);

    MyString &replace(int pos, int len, const MyString &v);

    MyString &replace(int pos, int len, const char *v);

    MyString &erase(int pos, int len);


    static int stoi(const MyString &str, int pos = 0, int base = 10);

    std::partial_ordering operator<=>(const MyString &other) const {
        return compare(*this , other) <=> 0;
    }

    static int compare(const MyString &lhs, const MyString &rhs);

    [[nodiscard]] MyString substr(int pos, int len) const;

    [[nodiscard]] int length() const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] char at(int pos) const;
    [[nodiscard]] char operator[](int pos) const;

    [[nodiscard]] const char * getData() const{
        return data;
    }
    
    [[nodiscard]] char * getData(){
        return data;
    }

    [[nodiscard]] int getCapacity() const {
        return capacity;
    }

    static std::istream &getline(std::istream &in, MyString &str, char delim = '\n');

    void resize(int newCapacity);

    static bool isWhitespace(char ch) {
        return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == EOF;
    }

private:
    static int charToInt(char c, int base);

};


inline std::ostream &operator<<(std::ostream &out, const MyString &string) {
    out << string.getData();
    return out;
}


inline std::istream &operator>>(std::istream &in, MyString &str) {
    char ch = 0;

    while (str.isEmpty() || !MyString::isWhitespace(ch)) {
        ch = in.get();
        if (!MyString::isWhitespace(ch)) {
            str.append(ch);
        }
    }
    return in;
}

