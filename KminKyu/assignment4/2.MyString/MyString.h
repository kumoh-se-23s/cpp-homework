#pragma once
#include<iostream>

class MyString {
public:
    MyString();

    MyString(const MyString &srcStr);

    MyString(const char str2nd[]);

    MyString& operator =(const MyString &srcStr);

    MyString& operator =(const char str2nd[]);

    int length() const;

    bool empty() const;

    int getCapacity() const;

    char at(int pos) const;

    char operator[](int pos) const;

    const MyString operator +(const char str2nd[]) const;

    const MyString operator +(const MyString& str2nd) const;

    bool operator ==(const MyString &str) const;

    bool operator !=(const MyString &str) const;

    int find(const char subStr[], int pos=0) const;

    int find(const MyString &subStr, int pos=0) const;

    MyString substr(int pos, int len) const;

    void append(char appendChr);

    void append(const char appendStr[]);

    void clear();

private:
    constexpr static int MAX_STR_SIZE = 15;
    char charArray[MAX_STR_SIZE + 1] = {};
    int size = 0;
    int capacity = MAX_STR_SIZE;

    void setString(const char str2nd[]);

    int rabinKarp(const char subStr[], int pos, int subStrLength) const;
    static bool isSame(const char str1st[], const char str2nd[], int start, int end);
};

std::ostream& operator <<(std::ostream& os, const MyString &str);
std::istream& operator >>(std::istream& is, MyString &str);
std::istream& getline(std::istream& is, MyString &str, char delim='\n');
