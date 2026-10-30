
#pragma once
#include <iostream>

class MyString{
    static constexpr int LEN = 15;
    char data[LEN + 1];
    int size = 0;

public:


    explicit MyString() = default;
    
    ~MyString() = default;

    MyString (const MyString &other);
    MyString &operator=(const char str2nd[]);
    MyString &operator=(const MyString &other);
    MyString(MyString &&other) = default;
    MyString &operator=(MyString &&other) = default;

    MyString operator+(const char str2nd[]);
    MyString operator+(const MyString &other);

    bool operator==(const MyString &str) const;
    bool operator!=(const MyString &str) const;
    [[nodiscard]] int find(const MyString &subStr, int pos = 0) const;
    [[nodiscard]] int find(const char *subStr, int pos = 0) const;
    [[nodiscard]] MyString subStr(int pos, int len) const;

    [[nodiscard]] int length() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] char at(int pos) const;
    [[nodiscard]] char operator[](int pos) const;

    [[nodiscard]] const char * getData() const{
        return data;
    }
    
    [[nodiscard]] char * getData(){
        return data;
    }

    static std::istream &getline(std::istream &in, MyString &str, char delim = '\n');

};
