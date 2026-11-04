//
// Created by apsode on 26. 11. 2..
//

#ifndef CPP_HOMEWORK_WEEK6_MYSTRING_H
#define CPP_HOMEWORK_WEEK6_MYSTRING_H
#include <iostream>
#include "CharSequence.h"
#include "Comparable.h"


class MyString : public CharSequence, Comparable<MyString>{
    public:
        MyString() = default;

        int length() const;

        bool isEmpty() const;

        int getCapacity() const;


        char& operator[](int pos);

        const char& operator[](int pos) const;


        MyString(const char *cstr2nd);

        MyString& operator=(const char *cstr2nd);

        MyString operator+(const char *cstr2nd) const;

        MyString operator+(const MyString &str2nd) const;

        bool operator==(const MyString &str) const;

        bool operator!=(const MyString &str) const;

        bool operator==(const char *cstr2nd) const;

        bool operator!=(const char *cstr2nd) const;

        int find(const char *cSubStr, int pos = 0) const;

        int find(const MyString &subStr, int pos = 0) const;

        MyString substr(int pos, int len) const;


        MyString& operator+=(const char *cstr2nd);

        MyString& operator+=(const MyString &str2nd);

        MyString& append(const char *cstr2nd);

        MyString& append(const MyString &str2nd);

        bool operator<(const MyString &str) const;

        bool operator<=(const MyString &str) const;

        bool operator>(const MyString &str) const;

        bool operator>=(const MyString &str) const;

        bool operator<(const char *cstr2nd) const;

        bool operator<=(const char *cstr2nd) const;

        bool operator>(const char *cstr2nd) const;

        bool operator>=(const char *cstr2nd) const;

        MyString& insert(int pos, const char *subStr);

        MyString& insert(int pos, const MyString &subStr);

        MyString& replace(int pos, int len, const char *subStr);

        MyString& replace(int pos, int len, const MyString &subStr);

        MyString& erase(int pos, int cnt);

        int compare_to(const MyString &other) const override;
};

std::istream& operator>>(std::istream &is, MyString &str);

std::istream& getline(std::istream &is, MyString &str, char delim = '\n');

int stoi(const MyString &str, int pos = 0, int base = 10);

#endif //CPP_HOMEWORK_WEEK6_MYSTRING_H
