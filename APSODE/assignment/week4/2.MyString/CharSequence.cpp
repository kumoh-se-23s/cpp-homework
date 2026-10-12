//
// Created by leegu on 26. 10. 10..
//
#include "CharSequence.h"


CharSequence::CharSequence()
    : container{}, length_(0) {
}

CharSequence::CharSequence(const CharSequence &other)
    : container{}, length_(0) {
    this->copy_from_other(other);
}

CharSequence::CharSequence(const char other[])
    : container{}, length_(0) {
    this->copy_from_other(other);
}

CharSequence::~CharSequence() {
    // 현재는 정적 배열 기반의 구현이므로 소멸자는 구현 X
    // 이후 동적 배열 기반으로 변경시 구현 예정
}

int CharSequence::length() const {
    return this->length_;
}

bool CharSequence::empty() const {
    return this->length_ == 0;
}

char CharSequence::at(const int pos) const {
    if (!this->is_correct_index_bound(pos)) {
        return '\0';
    }

    return this->container[pos];
}

bool CharSequence::set(const char object, const int pos) {
    if (!this->is_correct_index_bound(pos)) {
        return false;
    }

    this->container[pos] = object;
    return true;
}

void CharSequence::add_last(const char object) {
    if (this->length_ + 1 > MAX_LENGTH) {
        return;
    }

    this->container[this->length_++] = object;
}

bool CharSequence::equals(const CharSequence &other) const {
    if (this->length_ != other.length_) {
        return false;
    }

    for (int char_index = 0; char_index < this->length(); ++char_index) {
        if (this->container[char_index] != other.container[char_index]) {
            return false;
        }
    }

    return true;
}

int CharSequence::find(const CharSequence &pattern, const int start) const {
    return this->rabin_karp(pattern, start);
}

int CharSequence::find(const char pattern[], const int start) const {
    return this->rabin_karp(CharSequence(pattern), start);
}

void CharSequence::copy_from_other(const char other[]) {
    if (other[0] == '\0') {
        return this->flush();
    }
    this->length_ = 0;
    for (int char_index = 0; char_index < MAX_LENGTH && other[char_index] != '\0'; ++char_index) {
        this->container[char_index] = other[char_index];
        ++this->length_;
    }
}

void CharSequence::copy_from_other(const CharSequence &other) {
    this->copy_from_other(other.container);
}

bool CharSequence::is_correct_index_bound(const int index) const {
    return 0 <= index && index < this->length_;
}

CharSequence CharSequence::append_truncated(const char other[]) const {
    CharSequence appended_sequence = CharSequence(*this);

    if (other[0] == '\0') {
        return appended_sequence;
    }

    for (
        int this_index = this->length_, other_index = 0;
        this_index < MAX_LENGTH && other[other_index] != '\0';
        ++this_index, ++other_index
    ) {
        appended_sequence.container[this_index] = other[other_index];
        ++appended_sequence.length_;
    }

    return appended_sequence;
}

CharSequence CharSequence::append_truncated(const CharSequence &other) const {
    CharSequence appended_sequence = CharSequence(*this);

    if (other.empty()) {
        return appended_sequence;
    }

    for (
        int this_index = this->length_, other_index = 0;
        this_index < MAX_LENGTH && other_index < other.length_;
        ++this_index, ++other_index
    ) {
        appended_sequence.container[this_index] = other.container[other_index];
        ++appended_sequence.length_;
    }

    return appended_sequence;
}

int CharSequence::rabin_karp(const CharSequence &pattern, int start) const {
    start = start < 0 ? 0 : start;
    if (pattern.length_ == 0 || pattern.length_ + start > this->length_) {
        return -1;
    }

    // 최고차항에 곱해질 수
    long long int highest_power = 1;
    for (int loop = 1; loop < pattern.length_; ++loop) {
        highest_power *= HASH_BASE;
        highest_power %= HASH_MODULAR;
    }

    // 첫번째 윈도우의 해싱
    long long int hashed_pattern = 0, hashed_target = 0;
    for (int index = 0; index < pattern.length_; ++index) {
        hashed_pattern = (hashed_pattern * HASH_BASE + pattern.container[index]) % HASH_MODULAR;
        hashed_target = (hashed_target * HASH_BASE + pattern.container[start + index]) % HASH_MODULAR;
    }

    // 슬라이딩 윈도우 시작
    const int end = this->length_ - pattern.length_;
    for (int index = start; index <= end; ++index) {
        if (index != start) {
            hashed_target -= this->container[index - 1] * highest_power % HASH_MODULAR;

            if (hashed_target < 0) {
                hashed_target += HASH_MODULAR;
            }

            hashed_target += this->container[index + pattern.length_ - 1];
            hashed_target %= HASH_MODULAR;
        }

        if (hashed_pattern == hashed_target && is_same(*this, pattern, index)) {
            return index;
        }
    }

    return -1;
}

int CharSequence::is_same(const CharSequence &target, const CharSequence &pattern, const int pattern_start) {
    for (int index = 0; index < pattern.length_; ++index) {
        if (target.container[index + pattern_start] != pattern.container[index]) {
            return false;
        }
    }

    return true;
}

void CharSequence::flush() {
    this->length_ = 0;
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
