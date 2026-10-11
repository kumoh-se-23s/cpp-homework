//
// Created by leegu on 26. 10. 10..
//
#include "CharSequence.h"


CharSequence::CharSequence() : container{}, length_(0){
}

CharSequence::CharSequence(const CharSequence &other) : container{}, length_(0) {
    this->copy_from_other(other);
}

CharSequence::CharSequence(const char other[]) : container{}, length_(0){
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
