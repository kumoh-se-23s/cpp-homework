#pragma once
#include <iosfwd>

class MyString {
    public:
        static const int CAPACITY = 15;

        MyString();
        MyString(const MyString& srcStr);
        MyString(const char srcStr[]);

        int length() const;
        bool empty() const;
        char at(int pos) const;
        char operator[](int pos) const;


        int find(const char subStr[], int pos = 0) const;
        int find(const MyString& subStr, int pos = 0) const;

        MyString substr(int pos, int length) const;

        MyString& operator =(const MyString& srcStr);
        MyString& operator =(const char str2nd[]);

        const MyString operator +(const char str2nd[]) const;
        const MyString operator +(const MyString& srcStr) const;

        bool operator ==(const MyString& srcStr) const;
        bool operator !=(const MyString& srcStr) const;

    private:
        char str[CAPACITY + 1];
        int cstrlen(const char str[]) const;

};

std::ostream& operator<<(std::ostream& os, const MyString& str);
std::istream& operator>>(std::istream& is, MyString& str);
std::istream& getline(std::istream& is, MyString& str, char delimiter = '\n');
