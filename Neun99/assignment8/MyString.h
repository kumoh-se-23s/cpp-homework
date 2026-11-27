#pragma once
#include <iostream>

using namespace std;

class MyString {
public:
    //생성자--------------------
    MyString(); //초기 8byte, 빈 문자열
    MyString(int minLength);

    MyString(const MyString &srcStr); //복사생성자
    MyString(const char *cstr2nd); //cstring 복사생성자
    ~MyString(); //소멸자

    //기능----------------------
    int length() const;
    bool isEmpty() const;
    int getCapacity() const;
    char &at(int pos);
    const char &at(int pos) const;
    int find(const char *cSubStr, int pos = 0) const;
    int find(const MyString &subStr, int pos = 0) const;
    MyString substr(int pos, int len) const;
    void appendCh(char ch);
    MyString &append(const char *cSubStr);
    MyString &append(const MyString &subStr);
    MyString &insert(int pos, const char *csubStr);
    MyString &insert(int pos, const MyString &subStr);
    MyString &replace(int pos, int len, const char *csubStr);
    MyString &replace(int pos, int len, const MyString &subStr);
    MyString &erase(int pos, int cnt);
    MyString trim() const;
    void clear();

    //연산자 오버로딩-------------
    const MyString operator+(const char *cstr2nd) const;
    const MyString operator+(const MyString &str2nd) const;
    MyString &operator+=(const char *cstr2nd);
    MyString &operator+=(const MyString &str2nd);
    MyString &operator=(const char *cstr2nd);
    MyString &operator=(const MyString &srcStr);
    char &operator[](int pos);
    const char &operator[](int pos) const;

    bool operator==(const char *cstr2nd) const;
    bool operator==(const MyString &str2nd) const;
    bool operator!=(const char *cstr2nd) const;
    bool operator!=(const MyString &str2nd) const;
    bool operator<(const char *cstr2nd) const;
    bool operator<(const MyString &str2nd) const;
    bool operator>(const char *cstr2nd) const;
    bool operator>(const MyString &str2nd) const;
    bool operator<=(const char *cstr2nd) const;
    bool operator<=(const MyString &str2nd) const;
    bool operator>=(const char *cstr2nd) const;
    bool operator>=(const MyString &str2nd) const;

private:
    static constexpr int DEFAULT_CAPACITY = 8;
    int capacity = DEFAULT_CAPACITY;
    int size;
    char *str;

    void resize(int minLength);
    static void copyArr(char *copy, const char *src, int srcSize);
    static int getLengthOfChArr(const char *chArr);
    int getEnoughCapacity(int minLength) const;
    int normalizePos(int pos) const;
    int normalizeLen(int len, int pos) const;

    void substitute(const char *chArr, int newSize);
    int findChArr(const char *chArr, int arrSize, int pos) const;
    MyString subStrImpl(int pos, int len) const;
    const MyString plusImpl(const char *chArr, int chArrSize) const;
    void appendChArr(const char *chArr, int chArrSize);
    void insertChArr(const char *chArr, int chArrSize, int pos);
    void replaceChArr(const char *chArr, int chArrSize, int pos, int len);
    int compareTo(const char *chArr, int chArrSize) const;
    static int min(int val1, int val2);
};

ostream &operator<<(ostream &out, const MyString &str);
istream &operator>>(istream &in, MyString &str);
istream &getline(istream &in, MyString &str, char delim = '\n');
int stoi(const MyString &str, int pos = 0, int base = 10);
MyString itos(int val);
