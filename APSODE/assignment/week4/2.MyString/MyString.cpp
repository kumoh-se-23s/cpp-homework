//
// Created by leegu on 26. 10. 10..
//

#include "MyString.h"

MyString::MyString(const MyString &other) {
    this->copy_from_other(other);
}

MyString::~MyString() {
}

bool MyString::operator==(const MyString &other) const {
    return this->equals(other);
}

bool MyString::operator!=(const MyString &other) const {
    return !this->equals(other);
}

MyString& MyString::operator=(const MyString &other) {
    this->copy_from_other(other);

    return *this;
}

MyString MyString::operator+(const MyString &other) const {
    return MyString(this->append_truncated(other));
}

MyString MyString::operator+(const CharSequence &other) const {
    return MyString(this->append_truncated(other));
}

MyString MyString::operator+(const char char_array[]) const {
    return MyString(this->append_truncated(char_array));
}

MyString MyString::substr(const int pos, const int len) const {
    const int start = (pos < 0) ? 0 : pos;

    if (start >= this->m_length || len <= 0) {
        return MyString();
    }

    const int remaining = this->m_length - start;
    const int count = (len > remaining) ? remaining : len;

    MyString substring;
    for (int index = 0; index < count; ++index) {
        substring.add_last(this->container[start + index]);
    }

    return substring;
}

int MyString::find(const MyString &pattern, const int start) const {
    return CharSequence::find(pattern, start);
}

std::istream& operator>>(std::istream &input_stream, MyString &string) {
    MyString temp;
    int char_code = input_stream.get();

    // early return
    if (char_code == EOF) {
        return input_stream;
    }

    // strip lead\
    // TODO char_code 비교를 메소드화 고려해볼것.
    while (char_code == ' ' || char_code == '\t' || char_code == '\n') {
        char_code = input_stream.get();
    }

    for (;
        char_code != EOF;
        char_code = input_stream.get()
    ) {
        if (char_code == ' ' || char_code == '\n' || temp.length() >= CharSequence::MAX_LENGTH) {
            input_stream.putback(static_cast<char>(char_code));
            break;
        }

        temp.add_last(static_cast<char>(char_code));
    }

    temp.add_last('\0');
    string = temp;
    return input_stream;
}

std::istream& get_line(std::istream &input_stream, MyString &string, const char delim) {
    MyString temp;
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
            input_stream.ignore(10000, delim);
            break;
        }

        temp.add_last(static_cast<char>(char_code));
    }

    temp.add_last('\0');
    string = temp;
    return input_stream;
}
