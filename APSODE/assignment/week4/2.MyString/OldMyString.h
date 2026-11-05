//
// Created by leegu on 26. 10. 10..
//

#ifndef CPP_HOMEWORK_WEEK4_OldMyString_H
#define CPP_HOMEWORK_WEEK4_OldMyString_H
#include "CharSequence.h"

class OldMyString : public CharSequence {
    public:
        OldMyString() = default;

        explicit OldMyString(const CharSequence &other);

        explicit OldMyString(const char other[]);

        OldMyString(const OldMyString &other);

        ~OldMyString() override;


        OldMyString substr(int pos, int len) const;


        bool operator==(const OldMyString &other) const;

        bool operator!=(const OldMyString &other) const;

        OldMyString& operator=(const OldMyString &other);

        OldMyString operator+(const char char_array[]) const;

        OldMyString operator+(const CharSequence &other) const;

        OldMyString operator+(const OldMyString &other) const;
};

std::istream& operator>>(std::istream &input_stream, OldMyString &string);

std::istream& getline(std::istream &input_stream, OldMyString &string, char delim = '\n') ;
#endif //CPP_HOMEWORK_WEEK4_OldMyString_H
