#pragma once
#include <iostream>

class MyString {
    constexpr static int DEFAULT_CAPACITY = 8;


    char *charArray = nullptr;

    int size = 0;

    int capacity = 0;

public:
    MyString();

    MyString(const MyString &srcStr);

    MyString(const char *cstr2nd);

    MyString(MyString &&srcStr) noexcept;

    ~MyString();

    MyString &operator =(const MyString &srcStr);

    MyString &operator =(MyString &&srcStr);

    MyString &operator =(const char *cstr2nd);

    int length() const;

    bool empty() const;

    int getCapacity() const;

    char &at(int pos);

    const char &at(int pos) const;

    char &operator [](int pos);

    const char &operator [](int pos) const;

    MyString operator +(const char *cstr2nd) const;

    MyString operator +(const MyString &str2nd) const;

    bool operator ==(const MyString &str) const;

    bool operator !=(const MyString &str) const;

    bool operator ==(const char *cstr2nd) const;

    bool operator !=(const char *cstr2nd) const;

    int find(const char *cSubStr, int pos = 0) const;

    int find(const MyString &subStr, int pos = 0) const;

    MyString substr(int pos, int len) const;

    MyString &operator +=(const char *cstr2nd);

    MyString &operator +=(const MyString &str);

    MyString &append(char appendChr);

    MyString &append(const char *cstr2nd);

    MyString &append(const MyString &str);

    bool operator <(const MyString &str) const;

    bool operator >(const MyString &str) const;

    bool operator <=(const MyString &str) const;

    bool operator >=(const MyString &str) const;


    bool operator <(const char *cstr2nd) const;

    bool operator >(const char *cstr2nd) const;

    bool operator <=(const char *cstr2nd) const;

    bool operator >=(const char *cstr2nd) const;

    MyString &insert(int pos, const char *subStr);

    MyString &insert(int pos, const MyString &subStr);

    MyString &replace(int pos, int len, const char *subStr);

    MyString &replace(int pos, int len, const MyString &subStr);

    MyString &erase(int pos, int cnt);

    void clear();

private:

    void resize();

    //생성자, 대입 연산자에서 사용하는 string 복사 메소드
    void copyString(const char *copyStr, int copyStrCapacity);

    //비교 연산자에서 사용하는 메소드
    int compare(const char *str2nd) const;

    int rabinKarp(const char *subStr, int pos, int subStrLength) const;

    //insert, find, replace, erase 에서 올바른 pos 값을 지정하기 위한 메소드
    int positionNormalize(int pos) const;

    static bool isSame(const char *str1st, const char *str2nd, int start, int end);

    //생성자에서 cstr2nd의 사이즈에 맞는 배열 capacity를 찾아주기 위한 메소드
    static int growCapacity(const char *cstr2nd);
};

std::ostream &operator <<(std::ostream &os, const MyString &str);

std::istream &operator >>(std::istream &is, MyString &str);

std::istream &getline(std::istream &is, MyString &str, char delim = '\n');

int stoi(const MyString &str, int pos = 0, int base = 10);

//stoi에서 범위 안 알파벳을 int값으로 바꿔주는 메소드
int changeAlphabetToInt(char alphabet);
