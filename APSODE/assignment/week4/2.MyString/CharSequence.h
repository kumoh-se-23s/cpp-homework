//
// Created by leegu on 26. 10. 10..
//

#ifndef CPP_HOMEWORK_CHARSEQUENCE_H
#define CPP_HOMEWORK_CHARSEQUENCE_H
#include <iostream>

#include "Sequence.h"

class CharSequence : public Sequence<char> {
    public:
        constexpr static int MAX_LENGTH = 15;

        CharSequence();

        CharSequence(const CharSequence &other);

        CharSequence(const char other[]);

        ~CharSequence() override;


        int length() const override;

        char at(int pos) const override;

        bool set(char object, int pos) override;

        void add_last(char object) override;

        bool empty() const override;

        bool equals(const CharSequence &other) const;


        CharSequence append_truncated(const char other[]) const;

        CharSequence append_truncated(const CharSequence &other) const;

        void copy_from_other(const char other[]);

        void copy_from_other(const CharSequence &other);

    protected:
        char container[MAX_LENGTH + 1];

        int length_;

        bool is_correct_index_bound(int index) const;

        void flush();

    private:
        constexpr static int HASH_MODULAR = 1e9 + 7;
        constexpr static int HASH_BASE = 257;

        int rabin_karp(const CharSequence &pattern, int start) const;

        static int is_same(const CharSequence &target, const CharSequence &pattern, int pattern_start);

};

std::ostream& operator<<(std::ostream &output_stream, const CharSequence &char_sequence);

#endif //CPP_HOMEWORK_CHARSEQUENCE_H
