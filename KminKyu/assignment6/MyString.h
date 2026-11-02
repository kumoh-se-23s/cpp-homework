#pragma once
#include <iostream>

class MyString {
    constexpr static int DEFAULT_CAPACITY = 8;


    char *charArray;

    int size;

    int capacity;

public:
    MyString();

    MyString(const MyString &srcStr);

    MyString(const char *cstr2nd);

    MyString(MyString &&srcStr);

    ~MyString();

    MyString& operator =(const MyString &srcStr);

    MyString& operator =(MyString &&srcStr);

    MyString& operator =(const char *cstr2nd);

    int length() const;

    bool empty() const;

    int getCapacity() const;

    char& at(int pos);

    char& operator [](int pos);

    const char& operator [](int pos) const;

    MyString operator +(const char *cstr2nd) const;

    MyString operator +(const MyString& str2nd) const;

    bool operator ==(const MyString &str) const;

    bool operator !=(const MyString &str) const;

    bool operator ==(const char *cstr2nd) const;

    bool operator !=(const char *cstr2nd) const;

    int find(const char *cSubStr, int pos=0) const;

    int find(const MyString &subStr, int pos=0) const;

    MyString substr(int pos, int len) const;

    MyString& operator +=(const char *cstr2nd);

    MyString& operator +=(const MyString &str);

    MyString& append(char appendChr);

    MyString& append(const char *cstr2nd);

    MyString& append(const MyString &str);

    bool operator <(const MyString &str) const;
    bool operator >(const MyString &str) const;
    bool operator <=(const MyString &str) const;
    bool operator >=(const MyString &str) const;


    bool operator <(const char *cstr2nd) const;
    bool operator >(const char *cstr2nd) const;
    bool operator <=(const char *cstr2nd) const;
    bool operator >=(const char *cstr2nd) const;

    MyString& insert(int pos, const char *subStr);
    MyString& insert(int pos, const MyString &subStr);
    MyString& replace(int pos, int len, const char *subStr);
    MyString& replace(int pos, int len, const MyString& subStr);
    MyString& erase(int pos, int cnt);

    void clear();

private:
    void resize();
    void setString(const char *str2nd);
    int compare(const char *str2nd) const;
    int boyerMoore(const char *subStr, int pos, int subStrLength) const;
    static bool isSame(const char *str1st, const char str2nd[], int start, int end);\

};

std::ostream& operator <<(std::ostream& os, const MyString &str);
std::istream& operator >>(std::istream& is, MyString &str);
std::istream& getline(std::istream& is, MyString &str, char delim='\n');
int stoi(const MyString &str, int pos = 0, int base = 10);
int changeAlphabetToInt(char alphabet);

