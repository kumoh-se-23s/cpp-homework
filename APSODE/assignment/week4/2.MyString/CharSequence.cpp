//
// Created by leegu on 26. 10. 10..
//
#include "CharSequence.h"


CharSequence::CharSequence()
    : container{}, m_length(0) {
}

CharSequence::CharSequence(const CharSequence &other)
    : container{}, m_length(0) {
    this->copy_from_other(other);
}

CharSequence::CharSequence(const char other[])
    : container{}, m_length(0) {
    this->copy_from_other(other);
}

CharSequence::~CharSequence() {
    // 현재는 정적 배열 기반의 구현이므로 소멸자는 구현 X
    // 이후 동적 배열 기반으로 변경시 구현 예정
}

int CharSequence::length() const {
    return this->m_length;
}

bool CharSequence::empty() const {
    return this->m_length == 0;
}

char CharSequence::at(int pos) const {
    if (!this->is_correct_index_bound(pos)) {
        return '\0';
    }

    return this->container[pos];
}

bool CharSequence::set(char object, int pos) {
    if (!this->is_correct_index_bound(pos)) {
        return false;
    }

    this->container[pos] = object;
    return true;
}

void CharSequence::add_last(char object) {
    if (this->m_length + 1 > MAX_LENGTH) {
        return;
    }

    this->container[this->m_length++] = object;
}

bool CharSequence::equals(const CharSequence &other) const {
    if (this->m_length != other.m_length) {
        return false;
    }

    for (int char_index = 0; char_index < this->length(); ++char_index) {
        if (this->container[char_index] != other.container[char_index]) {
            return false;
        }
    }

    return true;
}

int CharSequence::find(const CharSequence &pattern, int start) const {
    return this->rabin_karp(pattern, start);
}

int CharSequence::find(const char pattern[], int start) const {
    return this->rabin_karp(CharSequence(pattern), start);
}

void CharSequence::copy_from_other(const char other[]) {
    if (other[0] == '\0') {
        return this->flush();
    }
    this->m_length = 0;
    for (int char_index = 0; char_index < MAX_LENGTH && other[char_index] != '\0'; ++char_index) {
        this->container[char_index] = other[char_index];
        ++this->m_length;
    }
}

void CharSequence::copy_from_other(const CharSequence &other) {
    this->copy_from_other(other.container);
}

bool CharSequence::is_correct_index_bound(int index) const {
    return 0 <= index && index < this->m_length;
}

CharSequence CharSequence::append_truncated(const char other[]) const {
    CharSequence appended_sequence = CharSequence(*this);

    if (other[0] == '\0') {
        return appended_sequence;
    }

    for (
        int this_index = this->m_length, other_index = 0;
        this_index < MAX_LENGTH && other[other_index] != '\0';
        ++this_index, ++other_index
    ) {
        appended_sequence.container[this_index] = other[other_index];
        ++appended_sequence.m_length;
    }

    return appended_sequence;
}

CharSequence CharSequence::append_truncated(const CharSequence &other) const {
    CharSequence appended_sequence = CharSequence(*this);

    if (other.empty()) {
        return appended_sequence;
    }

    for (
        int this_index = this->m_length, other_index = 0;
        this_index < MAX_LENGTH && other_index < other.m_length;
        ++this_index, ++other_index
    ) {
        appended_sequence.container[this_index] = other.container[other_index];
        ++appended_sequence.m_length;
    }

    return appended_sequence;
}

int CharSequence::rabin_karp(const CharSequence &pattern, int start) const {
    start = start < 0 ? 0 : start;
    if (pattern.m_length == 0 || pattern.m_length + start > this->m_length) {
        return -1;
    }

    // 최고차항에 곱해질 수
    long long int highest_power = 1;
    for (int loop = 1; loop < pattern.m_length; ++loop) {
        highest_power *= HASH_BASE;
        highest_power %= HASH_MODULAR;
    }

    // 첫번째 윈도우의 해싱
    long long int hashed_pattern = 0, hashed_target = 0;
    for (int index = 0; index < pattern.m_length; ++index) {
        hashed_pattern = (hashed_pattern * HASH_BASE + pattern.container[index]) % HASH_MODULAR;
        hashed_target = (hashed_target * HASH_BASE + this->container[start + index]) % HASH_MODULAR;
    }

    // 슬라이딩 윈도우 시작
    const int end = this->m_length - pattern.m_length;
    for (int index = start; index <= end; ++index) {
        if (index != start) {
            hashed_target -= this->container[index - 1] * highest_power % HASH_MODULAR;

            if (hashed_target < 0) {
                hashed_target += HASH_MODULAR;
            }

            hashed_target *= HASH_BASE;


            hashed_target += this->container[index + pattern.m_length - 1];
            hashed_target %= HASH_MODULAR;
        }

        if (hashed_pattern == hashed_target && is_same(*this, pattern, index)) {
            return index;
        }
    }

    return -1;
}

bool CharSequence::is_same(const CharSequence &target, const CharSequence &pattern, int pattern_start) {
    for (int index = 0; index < pattern.m_length; ++index) {
        if (target.container[index + pattern_start] != pattern.container[index]) {
            return false;
        }
    }

    return true;
}

void CharSequence::flush() {
    this->m_length = 0;
    for (int char_index = 0; char_index < MAX_LENGTH; ++char_index) {
        this->container[char_index] = '\0';
    }
}

std::ostream& operator<<(std::ostream &output_stream, const CharSequence &char_sequence) {
    for (int index = 0; index < char_sequence.length(); ++index) {
        output_stream << char_sequence.at(index);
    }
    return output_stream;
}
