
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
    int find(const MyString &subStr, int pos = 0) const;
    int find(const char *subStr, int pos = 0) const;
    MyString subStr(int pos, int len) const;

    int length() const;
    bool empty() const;
    char at(int pos) const;
    char operator[](int pos) const;

    const char * getData() const{
        return data;
    }
    
    char * getData(){
        return data;
    }

    static std::istream &getline(std::istream &in, MyString &str, char delim = '\n');

};


inline std::ostream &operator<<(std::ostream &out, const MyString &string){
    out << string.getData();
    return out;
}


inline std::istream &operator>>(std::istream &in, MyString &string){
    static constexpr int LEN = 15;
    MyString::getline(in, string);
    return in;
}