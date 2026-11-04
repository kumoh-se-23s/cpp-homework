//
// Created by apsode on 26. 11. 3..
//

#ifndef CPP_HOMEWORK_WEEK6_CHARSEQUENCE_H
#define CPP_HOMEWORK_WEEK6_CHARSEQUENCE_H
#include <iostream>
#include "Sequence.h"


class CharSequence : public Sequence<char> {
    constexpr static int HASH_MODULAR = 1e9 + 7;
    constexpr static int HASH_BASE = 257;

    public:
        CharSequence() = default;

        CharSequence(const CharSequence &other) = default;

        CharSequence(const char *elements, int size);

        ~CharSequence() override = default;


        int find(const CharSequence &pattern, int start) const;

        int find(const char *pattern, int start) const;

    private:
        int rabin_karp(const CharSequence &pattern, int start) const;

        static bool is_same(const CharSequence &target, const CharSequence &pattern, int pattern_start);
};

std::ostream& operator<<(std::ostream &output_stream, const CharSequence &char_sequence);

#endif //CPP_HOMEWORK_WEEK6_CHARSEQUENCE_H
