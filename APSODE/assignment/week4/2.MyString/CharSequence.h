//
// Created by leegu on 26. 10. 10..
//

#ifndef CPP_HOMEWORK_CHARSEQUENCE_H
#define CPP_HOMEWORK_CHARSEQUENCE_H
#include <iostream>

class CharSequence {
    public:
        virtual ~CharSequence() = default;

        virtual int length() const = 0;

        virtual char at(int pos) const = 0;

        virtual void set(char target_char, int pos) = 0;

        virtual bool empty() const = 0;
};

std::ostream &operator<<(std::ostream &output_stream, const CharSequence &char_sequence);

#endif //CPP_HOMEWORK_CHARSEQUENCE_H
