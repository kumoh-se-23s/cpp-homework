//
// Created by apsode on 26. 11. 2..
//

#ifndef CPP_HOMEWORK_WEEK6_MYSTRING_H
#define CPP_HOMEWORK_WEEK6_MYSTRING_H
#include <iostream>
#include "CharSequence.h"
#include "Comparable.h"


class MyString : public CharSequence, Comparable<MyString> {
    public:
        MyString() = default;

        MyString(const CharSequence& sequence);

        MyString(const char *cstr2nd);
        using CharSequence::append;
        // using CharSequence::insert;
        // using CharSequence::compare;
        // using CharSequence::find;

        int length() const;

        bool isEmpty() const;

        int getCapacity() const;

        int find(const MyString &subStr, int pos = 0) const;

        MyString substr(int pos, int len) const;

        MyString& append(const MyString &str2nd);

        MyString& insert(int pos, const MyString &subStr);

        MyString& replace(int pos, int len, const MyString &subStr);

        MyString& replace(int pos, int len, const char *subStr);

        MyString& erase(int pos, int cnt);

        int compare(const MyString &other) const override;


        MyString operator+(const MyString &str2nd) const;

        MyString& operator+=(const MyString &str2nd);
};

std::istream& operator>>(std::istream &is, MyString &str);

std::istream& getline(std::istream &is, MyString &str, char delim = '\n');

int stoi(const MyString &str, int pos = 0, int base = 10);

#endif //CPP_HOMEWORK_WEEK6_MYSTRING_H
