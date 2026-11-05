//
// Created by apsode on 26. 11. 2..
//

#include "MyString.h"

#include <limits>

MyString::MyString(const CharSequence &sequence) {
}

MyString::MyString(const char *cstr2nd) {
}

int MyString::length() const {
    return this->m_size;
}

bool MyString::isEmpty() const {
    return this->m_size == 0;
}

int MyString::getCapacity() const {
    return this->m_capacity;
}

int MyString::find(const MyString &subStr, int pos) const {
    return CharSequence::find(subStr, pos);
}

MyString MyString::substr(int pos, int len) const {
    return MyString("TODO");
}

MyString& MyString::append(const MyString &str2nd) {
    CharSequence::append(str2nd);
    return *this;
}

MyString& MyString::insert(int pos, const MyString &subStr) {
    CharSequence::insert(pos, subStr);
    return *this;
}

MyString& MyString::replace(int pos, int len, const MyString &subStr) {
    Sequence::replace(pos, len, subStr.container);
    return *this;
}

MyString& MyString::replace(int pos, int len, const char *subStr) {
    Sequence::replace(pos, len, subStr);
    return *this;
}


MyString& MyString::erase(int pos, int cnt) {
    Sequence::erase(pos, cnt);
    return *this;
}

int MyString::compare(const MyString &other) const {
    return CharSequence::compare(other);
}

MyString MyString::operator+(const MyString &str2nd) const {
    return MyString(*this).append(str2nd);
}

MyString& MyString::operator+=(const MyString &str2nd) {
    return this->append(str2nd);
}

std::istream& operator>>(std::istream &input_stream, MyString &string) {
    MyString temp;
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
        if (std::isspace(char_code) || temp.length() >= string.capacity()) {
            input_stream.putback(static_cast<char>(char_code));
            break;
        }

        temp.CharSequence::append(static_cast<char>(char_code));
    }

    temp.CharSequence::append('\0');
    string = temp;
    return input_stream;
}

std::istream& getline(std::istream &input_stream, MyString &string, char delim) {
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
        if (temp.length() >= string.getCapacity()) {
            input_stream.ignore(std::numeric_limits<std::streamsize>::max(), delim);
            break;
        }

        temp.CharSequence::append(static_cast<char>(char_code));
    }

    temp.CharSequence::append('\0');
    string = temp;
    return input_stream;
}

int stoi(const MyString &str, int pos, int base) {
    if (0 <= pos && pos < str.length()) {

    }
}
