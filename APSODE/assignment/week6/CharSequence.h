//
// Created by apsode on 26. 11. 3..
//

#ifndef CPP_HOMEWORK_WEEK6_CHARSEQUENCE_H
#define CPP_HOMEWORK_WEEK6_CHARSEQUENCE_H
#include <iostream>

#include "Comparable.h"
#include "Sequence.h"


class CharSequence : public Sequence<char>, public Comparable<const char*> {
    constexpr static int HASH_MODULAR = 1e9 + 7;
    constexpr static int HASH_BASE = 257;

    public:
        CharSequence() = default;

        CharSequence(const CharSequence &other) = default;

        CharSequence(const char *elements, int size);

        ~CharSequence() override = default;


        int find(const CharSequence &pattern, int start) const;

        int find(const char *pattern, int start) const;

        void append(const char* chars);

        void append(const CharSequence &other);

        void insert(int pos, const char* chars);

        void insert(int pos, const CharSequence &other);

        int compare(const char * const &other) const override;

        char& operator[](int pos);

        const char& operator[](int pos) const;


        CharSequence operator+(const char* chars) const;

        CharSequence& operator=(const char* chars);

        CharSequence& operator+=(const char* chars);

    protected:
        static int count_chars(const char *chars);

    private:
        int rabin_karp(const CharSequence &pattern, int start) const;

        static bool is_same(const CharSequence &target, const CharSequence &pattern, int pattern_start);
};

std::ostream& operator<<(std::ostream &output_stream, const CharSequence &char_sequence);

#endif //CPP_HOMEWORK_WEEK6_CHARSEQUENCE_H
