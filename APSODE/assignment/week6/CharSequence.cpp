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

void CharSequence::append(const char *chars) {
    require_non_null(chars);
    Sequence::append(chars, count_elements(chars));
}

void CharSequence::append(const CharSequence &other) {
    Sequence::append(other.container, other.m_size);
}

void CharSequence::insert(int pos, const char *chars) {
    require_non_null(chars);
    Sequence::insert(pos, chars, count_elements(chars));
}

void CharSequence::insert(int pos, const CharSequence &other) {
    Sequence::insert(pos, other.container, other.m_size);
}

char& CharSequence::operator[](int pos) {
    return Sequence::operator[](pos);
}

const char& CharSequence::operator[](int pos) const {
    return Sequence::operator[](pos);
}

CharSequence CharSequence::operator+(const char *chars) const {
    require_non_null(chars);
    CharSequence temp = CharSequence(*this);
    temp.append(chars);
    return temp;
}

CharSequence& CharSequence::operator=(const char *chars) {
    require_non_null(chars);
    swap(*this, CharSequence(chars, count_elements(chars)));
    return *this;
}

CharSequence& CharSequence::operator+=(const char *chars) {
    require_non_null(chars);
    swap(*this, *this + chars);
    return *this;
}

bool CharSequence::operator==(const char *chars) const {
    require_non_null(chars);
    return this->equals(CharSequence(chars, count_elements(chars)));
}

bool CharSequence::operator!=(const char *chars) const {
    return !(*this == chars);
}

bool CharSequence::operator>(const char *chars) const {
    if (chars == nullptr) {
        return true;
    }

    int chars_length = count_elements(chars);

    for (int index = 0; index < this->m_size && index < chars_length; ++index) {
        unsigned char this_char = (*this)[index];
        unsigned char other_char = *chars++;
        if (this_char != other_char) {
            return this_char > other_char;
        }
    }

    return this->m_size > chars_length;
}

bool CharSequence::operator<(const char *chars) const {
    return !(*this >= chars);
}

bool CharSequence::operator>=(const char *chars) const {
    return *this > chars || *this == chars;
}

bool CharSequence::operator<=(const char *chars) const {
    return *this < chars || *this == chars;
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
        hashed_pattern = (hashed_pattern * HASH_BASE + static_cast<unsigned char>(pattern.container[index])) % HASH_MODULAR;
        hashed_target = (hashed_target * HASH_BASE + static_cast<unsigned char>(this->container[start + index])) % HASH_MODULAR;
    }

    // 슬라이딩 윈도우 시작
    const int end = this->m_size - pattern.m_size;
    for (int index = start; index <= end; ++index) {
        if (index != start) {
            hashed_target -= static_cast<unsigned char>(this->container[index - 1]) * highest_power % HASH_MODULAR;

            if (hashed_target < 0) {
                hashed_target += HASH_MODULAR;
            }

            hashed_target *= HASH_BASE;


            hashed_target += static_cast<unsigned char>(this->container[index + pattern.m_size - 1]);
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
