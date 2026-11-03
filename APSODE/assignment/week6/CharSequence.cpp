//
// Created by apsode on 26. 11. 3..
//

#include "CharSequence.h"



int CharSequence::find(const CharSequence &pattern, int start) const {
    return this->rabin_karp(pattern, start);
}

int CharSequence::find(const char pattern[], int start) const {
    return this->rabin_karp(CharSequence(pattern), start);
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
