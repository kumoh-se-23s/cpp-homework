#pragma once
#include<iostream>

class OOldMyString {
public:
    OOldMyString();

    OOldMyString(const OOldMyString &srcStr);

    OOldMyString(const char str2nd[]);

    OOldMyString& operator =(const OOldMyString &srcStr);

    OOldMyString& operator =(const char str2nd[]);

    int length() const;

    bool empty() const;

    int getCapacity() const;

    char at(int pos) const;

    char operator[](int pos) const;

    const OOldMyString operator +(const char str2nd[]) const;

    const OOldMyString operator +(const OOldMyString& str2nd) const;

    bool operator ==(const OOldMyString &str) const;

    bool operator !=(const OOldMyString &str) const;

    int find(const char subStr[], int pos=0) const;

    int find(const OOldMyString &subStr, int pos=0) const;

    OOldMyString substr(int pos, int len) const;

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

std::ostream& operator <<(std::ostream& os, const OOldMyString &str);
std::istream& operator >>(std::istream& is, OOldMyString &str);
std::istream& getline(std::istream& is, OOldMyString &str, char delim='\n');
