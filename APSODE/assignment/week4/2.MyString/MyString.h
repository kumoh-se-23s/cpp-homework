//
// Created by leegu on 26. 10. 10..
//

#ifndef CPP_HOMEWORK_MYSTRING_H
#define CPP_HOMEWORK_MYSTRING_H
#include "CharSequence.h"

class MyString : public CharSequence {
    public:
        MyString() = default;

        explicit MyString(const CharSequence &other)
            : CharSequence(other) {
        }

        explicit MyString(const char other[])
            : CharSequence(other) {
        }

        MyString(const MyString &other);

        ~MyString() override;


        bool operator==(const MyString &other) const;

        bool operator!=(const MyString &other) const;

        MyString& operator=(const MyString &other);

        MyString operator+(const MyString &other) const;

        MyString operator+(const CharSequence &other) const;

        MyString operator+(const char char_array[]) const;

        MyString substr(int pos, int len) const;
};

std::istream& operator>>(std::istream &input_stream, MyString &string);

std::istream& get_line(std::istream &input_stream, MyString &string, char delim = '\n') ;
#endif //CPP_HOMEWORK_MYSTRING_H
