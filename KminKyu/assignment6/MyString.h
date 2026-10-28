#pragma once
#include<iostream>

class MyString {
public:
    MyString();

    MyString(const MyString &srcStr);

    MyString(const char *cstr2nd[]);

    ~MyString();

    MyString& operator =(const MyString &srcStr);

    MyString& operator =(const char *cstr2nd);

    int length() const;

    bool empty() const;

    int getCapacity() const;

    char& at(int pos) const;

    char& operator[](int pos) const;

    const char& operator[](int pos) const;

    const MyString operator +(const char *cstr2nd) const;

    const MyString operator +(const MyString& str2nd) const;

    bool operator ==(const MyString &str) const;

    bool operator !=(const MyString &str) const;

    bool operator ==(const char* cstr2nd) const;

    bool operator !=(const char* str2nd) const;

    int find(const char *cSubStr, int pos=0) const;

    int find(const MyString &subStr, int pos=0) const;

    MyString substr(int pos, int len) const;

    MyString& operator +=(const char* cstr2nd);

    MyString& operator +=(const MyString &str);

    MyString& append(char appendChr);

    MyString& append(const char* cstr2nd);

    MyString& append(const MyString &str);

    bool operator <(const MyString &str) const;
    bool operator >(const MyString &str) const;
    bool operator <=(const MyString &str) const;
    bool operator >=(const MyString &str) const;


    bool operator <(const char& cstr2nd) const;
    bool operator >(const char& cstr2nd) const;
    bool operator <=(const char& cstr2nd) const;
    bool operator >=(const char& cstr2nd) const;

    insert(int pos, const char *subStr);
    insert(int pos, const MyString &subStr);
    replace(int pos, int len, const char *subStr);
    replace(int pos, int len, const MyString& subStr);
    erase(int pos, int cnt);

    void clear();

private:
    constexpr static int MAX_STR_SIZE = 15;
    char charArray[MAX_STR_SIZE + 1] = {};
    int size = 0;
    int capacity = MAX_STR_SIZE;

    void setString(const char str2nd[]);

    int boyerMoore(const char subStr[], int pos, int subStrLength) const;
    static bool isSame(const char str1st[], const char str2nd[], int start, int end);
};

std::ostream& operator <<(std::ostream& os, const MyString &str);
std::istream& operator >>(std::istream& is, MyString &str);
std::istream& getline(std::istream& is, MyString &str, char delim='\n');
int stoi(const MyString &str, int pos = 0, int base = 10);

