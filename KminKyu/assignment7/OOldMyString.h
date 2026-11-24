#pragma once
#include <iostream>

class OOldMyString {
    constexpr static int DEFAULT_CAPACITY = 8;


    char *charArray = nullptr;

    int size = 0;

    int capacity = 0;

public:
    OOldMyString();

    OOldMyString(const OOldMyString &srcStr);

    OOldMyString(const char *cstr2nd);

    OOldMyString(OOldMyString &&srcStr) noexcept;

    ~OOldMyString();

    OOldMyString &operator =(const OOldMyString &srcStr);

    OOldMyString &operator =(OOldMyString &&srcStr) noexcept;

    OOldMyString &operator =(const char *cstr2nd);

    int length() const;

    bool isEmpty() const;

    int getCapacity() const;

    char &at(int pos);

    const char &at(int pos) const;

    char &operator [](int pos);

    const char &operator [](int pos) const;

    OOldMyString operator +(const char *cstr2nd) const;

    OOldMyString operator +(const OOldMyString &str2nd) const;

    bool operator ==(const OOldMyString &str) const;

    bool operator !=(const OOldMyString &str) const;

    bool operator ==(const char *cstr2nd) const;

    bool operator !=(const char *cstr2nd) const;

    int find(const char *cSubStr, int pos = 0) const;

    int find(const OOldMyString &subStr, int pos = 0) const;

    OOldMyString substr(int pos, int len) const;

    OOldMyString &operator +=(const char *cstr2nd);

    OOldMyString &operator +=(const OOldMyString &str);

    OOldMyString &append(char appendChr);

    OOldMyString &append(const char *cstr2nd);

    OOldMyString &append(const OOldMyString &str);

    bool operator <(const OOldMyString &str) const;

    bool operator >(const OOldMyString &str) const;

    bool operator <=(const OOldMyString &str) const;

    bool operator >=(const OOldMyString &str) const;


    bool operator <(const char *cstr2nd) const;

    bool operator >(const char *cstr2nd) const;

    bool operator <=(const char *cstr2nd) const;

    bool operator >=(const char *cstr2nd) const;

    OOldMyString &insert(int pos, const char *subStr);

    OOldMyString &insert(int pos, const OOldMyString &subStr);

    OOldMyString &replace(int pos, int len, const char *subStr);

    OOldMyString &replace(int pos, int len, const OOldMyString &subStr);

    OOldMyString &erase(int pos, int cnt);

    void clear();

    static OOldMyString integerToString(int integer);

    OOldMyString& trim();

private:

    void resize();

    void resize(int reCapacity);

    //배열 길이를 구해서 인자로 private 메소드 -> 따로 nullptr 체크는 하지 않음
    OOldMyString &insert(int pos, const char *subStr, int subStrSize);

    OOldMyString &replace(int pos, int len, const char *subStr, int subStrSize);

    OOldMyString &append(const char *cstr2nd, int cstr2ndSize);

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

std::ostream &operator <<(std::ostream &os, const OOldMyString &str);

std::istream &operator >>(std::istream &is, OOldMyString &str);

std::istream &getline(std::istream &is, OOldMyString &str, char delim = '\n');

int stoi(const OOldMyString &str, int pos = 0, int base = 10);

//stoi에서 범위 안 알파벳을 int값으로 바꿔주는 메소드
int changeAlphabetToInt(char alphabet);
