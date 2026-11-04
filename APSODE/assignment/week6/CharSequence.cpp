//
// Created by apsode on 26. 11. 3..
//

#include "CharSequence.h"


CharSequence::CharSequence(const char *elements, int size)
    : Sequence(elements, size) {
}

int CharSequence::find(const CharSequence &pattern, int start) const {
    return this->rabin_karp(pattern, start);
}

int CharSequence::find(const char *pattern, int start) const {
    return this->rabin_karp(CharSequence(pattern, count_elements(pattern)), start);
}

CharSequence& CharSequence::append(const char *chars) {
    require_non_null(chars);
    Sequence::append(chars, count_elements(chars));
    return *this;
}

CharSequence& CharSequence::insert(int pos, const char *chars) {
    require_non_null(chars);
    Sequence::insert(pos, chars, count_elements(chars));
    return *this;
}

char& CharSequence::operator[](int pos) {
    return Sequence::operator[](pos);
}

const char& CharSequence::operator[](int pos) const {
    return Sequence::operator[](pos);
}

CharSequence CharSequence::operator+(const char *chars) const {
    require_non_null(chars);
    return CharSequence(*this).append(chars);
}

CharSequence& CharSequence::operator=(const char *chars) {

}

CharSequence& CharSequence::operator+=(const char *chars) {
}

bool CharSequence::operator==(const char *chars) const {
}

bool CharSequence::operator!=(const char *chars) const {
}

bool CharSequence::operator>(const char *chars) const {
}

bool CharSequence::operator<(const char *chars) const {
}

bool CharSequence::operator>=(const char *chars) const {
}

bool CharSequence::operator<=(const char *chars) const {
}

int CharSequence::rabin_karp(const CharSequence &pattern, int start) const {
    start = start < 0 ? 0 : start;
    if (pattern.m_size == 0 || pattern.m_size + start > this->m_size) {
        return -1;
    }

    // 최고차항에 곱해질 수
    long long int highest_power = 1;
    for (int loop = 1; loop < pattern.m_size; ++loop) {
        highest_power *= HASH_BASE;
        highest_power %= HASH_MODULAR;
    }

    // 첫번째 윈도우의 해싱
    long long int hashed_pattern = 0, hashed_target = 0;
    for (int index = 0; index < pattern.m_size; ++index) {
        hashed_pattern = (hashed_pattern * HASH_BASE + pattern.container[index]) % HASH_MODULAR;
        hashed_target = (hashed_target * HASH_BASE + this->container[start + index]) % HASH_MODULAR;
    }

    // 슬라이딩 윈도우 시작
    const int end = this->m_size - pattern.m_size;
    for (int index = start; index <= end; ++index) {
        if (index != start) {
            hashed_target -= this->container[index - 1] * highest_power % HASH_MODULAR;

            if (hashed_target < 0) {
                hashed_target += HASH_MODULAR;
            }

            hashed_target *= HASH_BASE;


            hashed_target += this->container[index + pattern.m_size - 1];
            hashed_target %= HASH_MODULAR;
        }

        if (hashed_pattern == hashed_target && is_same(*this, pattern, index)) {
            return index;
        }
    }

    return -1;
}

bool CharSequence::is_same(const CharSequence &target, const CharSequence &pattern, int pattern_start) {
    for (int index = 0; index < pattern.m_size; ++index) {
        if (target.container[index + pattern_start] != pattern.container[index]) {
            return false;
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream &output_stream, const CharSequence &char_sequence) {
    for (int index = 0; index < char_sequence.size(); ++index) {
        output_stream << char_sequence.at(index);
    }
    return output_stream;
}
