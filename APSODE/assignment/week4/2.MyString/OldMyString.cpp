//
// Created by leegu on 26. 10. 10..
//

#include "OldMyString.h"

#include <limits>

OldMyString::OldMyString(const OldMyString &other) : CharSequence(other) {
}

OldMyString::OldMyString(const CharSequence &other) : CharSequence(other){
}

OldMyString::OldMyString(const char other[]) : CharSequence(other){
}

OldMyString::~OldMyString() = default;

OldMyString OldMyString::substr(int pos, int len) const {
    const int start = (pos < 0) ? 0 : pos;

    if (start >= this->m_length || len <= 0) {
        return OldMyString();
    }

    const int remaining = this->m_length - start;
    const int count = (len > remaining) ? remaining : len;

    OldMyString substring;
    for (int index = 0; index < count; ++index) {
        substring.add_last(this->container[start + index]);
    }

    return substring;
}

bool OldMyString::operator==(const OldMyString &other) const {
    return this->equals(other);
}

bool OldMyString::operator!=(const OldMyString &other) const {
    return !this->equals(other);
}

OldMyString& OldMyString::operator=(const OldMyString &other) {
    this->copy_from_other(other);

    return *this;
}

OldMyString OldMyString::operator+(const OldMyString &other) const {
    return OldMyString(this->append_truncated(other));
}

OldMyString OldMyString::operator+(const CharSequence &other) const {
    return OldMyString(this->append_truncated(other));
}

OldMyString OldMyString::operator+(const char char_array[]) const {
    return OldMyString(this->append_truncated(char_array));
}

std::istream& operator>>(std::istream &input_stream, OldMyString &string) {
    OldMyString temp;
    int char_code = input_stream.get();

    // EOF를 검증을 해줘야할까..?

    // early return
    if (char_code == EOF) {
        return input_stream;
    }

    // strip lead
    while (std::isspace(char_code)) {
        char_code = input_stream.get();
    }

    for (;
        char_code != EOF;
        char_code = input_stream.get()
    ) {
        if (std::isspace(char_code) || temp.length() >= CharSequence::MAX_LENGTH) {
            input_stream.putback(static_cast<char>(char_code));
            break;
        }

        temp.add_last(static_cast<char>(char_code));
    }

    temp.add_last('\0');
    string = temp;
    return input_stream;
}

std::istream& getline(std::istream &input_stream, OldMyString &string, char delim) {
    OldMyString temp;
    int char_code = input_stream.get();

    // early return
    if (char_code == EOF) {
        return input_stream;
    }

    for (;
        char_code != EOF && char_code != delim;
        char_code = input_stream.get()
    ) {
        if (temp.length() >= CharSequence::MAX_LENGTH) {
            input_stream.ignore(std::numeric_limits<std::streamsize>::max(), delim);
            break;
        }

        temp.add_last(static_cast<char>(char_code));
    }

    temp.add_last('\0');
    string = temp;
    return input_stream;
}
