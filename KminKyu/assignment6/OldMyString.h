#pragma once
#include <iostream>

class OldMyString {
    constexpr static int DEFAULT_CAPACITY = 8;


    char *charArray = nullptr;

    int size = 0;

    int capacity = 0;

public:
    OldMyString();

    OldMyString(const OldMyString &srcStr);

    OldMyString(const char *cstr2nd);

    OldMyString(OldMyString &&srcStr) noexcept;

    ~OldMyString();

    OldMyString &operator =(const OldMyString &srcStr);

    OldMyString &operator =(OldMyString &&srcStr) noexcept;

    OldMyString &operator =(const char *cstr2nd);

    int length() const;

    bool isEmpty() const;

    int getCapacity() const;

    char &at(int pos);

    const char &at(int pos) const;

    char &operator [](int pos);

    const char &operator [](int pos) const;

    OldMyString operator +(const char *cstr2nd) const;

    OldMyString operator +(const OldMyString &str2nd) const;

    bool operator ==(const OldMyString &str) const;

    bool operator !=(const OldMyString &str) const;

    bool operator ==(const char *cstr2nd) const;

    bool operator !=(const char *cstr2nd) const;

    int find(const char *cSubStr, int pos = 0) const;

    int find(const OldMyString &subStr, int pos = 0) const;

    OldMyString substr(int pos, int len) const;

    OldMyString &operator +=(const char *cstr2nd);

    OldMyString &operator +=(const OldMyString &str);

    OldMyString &append(char appendChr);

    OldMyString &append(const char *cstr2nd);

    OldMyString &append(const OldMyString &str);

    bool operator <(const OldMyString &str) const;

    bool operator >(const OldMyString &str) const;

    bool operator <=(const OldMyString &str) const;

    bool operator >=(const OldMyString &str) const;


    bool operator <(const char *cstr2nd) const;

    bool operator >(const char *cstr2nd) const;

    bool operator <=(const char *cstr2nd) const;

    bool operator >=(const char *cstr2nd) const;

    OldMyString &insert(int pos, const char *subStr);

    OldMyString &insert(int pos, const OldMyString &subStr);

    OldMyString &replace(int pos, int len, const char *subStr);

    OldMyString &replace(int pos, int len, const OldMyString &subStr);

    OldMyString &erase(int pos, int cnt);

    void clear();

private:

    void resize();

    void resize(int reCapacity);

    //배열 길이를 구해서 인자로 private 메소드 -> 따로 nullptr 체크는 하지 않음
    OldMyString &insert(int pos, const char *subStr, int subStrSize);

    OldMyString &replace(int pos, int len, const char *subStr, int subStrSize);

    OldMyString &append(const char *cstr2nd, int cstr2ndSize);

    static int getCharArraySize(const char *str2nd);

    static char *copyCharArray(const char *baseStr, int baseStrSize);
    //생성자, 대입 연산자에서 사용하는 string 복사 메소드
    void copyString(const char *copyStr, int copyStrCapacity);

    //비교 연산자에서 사용하는 메소드
    int compare(const char *str2nd) const;

    int rabinKarp(const char *subStr, int pos, int subStrSize) const;

    //insert, find, replace, erase 에서 올바른 pos 값을 지정하기 위한 메소드
    int positionNormalize(int pos) const;

    int lengthNormalize(int pos, int len) const;

    static bool isSame(const char *str1st, const char *str2nd, int start, int end);

    //생성자에서 cstr2nd의 사이즈에 맞는 배열 capacity를 찾아주기 위한 메소드
    static int calcInitialCapacity(int arraySize);
};

std::ostream &operator <<(std::ostream &os, const OldMyString &str);

std::istream &operator >>(std::istream &is, OldMyString &str);

std::istream &getline(std::istream &is, OldMyString &str, char delim = '\n');

int stoi(const OldMyString &str, int pos = 0, int base = 10);

//stoi에서 범위 안 알파벳을 int값으로 바꿔주는 메소드
int changeAlphabetToInt(char alphabet);
