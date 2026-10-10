//
// Created by leegu on 26. 10. 10..
//

#ifndef CPP_HOMEWORK_MYSTRING_H
#define CPP_HOMEWORK_MYSTRING_H
#include "CharSequence.h"

class String : public CharSequence {
    public:
        constexpr static int MAX_LENGTH = 15;

        String();

        String(const char other_char_array[]);

        String(const String &other);

        ~String() override;


        int length() const override;

        char at(int pos) const override;

        bool empty() const override;

        void set(char target_char, int pos) override;


        bool operator==(const String &other) const;

        String operator=(const String &other);

        String operator+(const String &other) const;

        String operator+(const CharSequence &other) const;

        String operator+(const char char_array[]) const;

    private:
        char container[MAX_LENGTH + 1];

        int length;

        void copy_from_other(const CharSequence &other);

        void copy_from_other(const char other[]);
};

std::istream &operator>>(std::istream &input_stream, String &string);
#endif //CPP_HOMEWORK_MYSTRING_H

