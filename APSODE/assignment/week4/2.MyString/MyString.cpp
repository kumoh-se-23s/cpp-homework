//
// Created by leegu on 26. 10. 10..
//

#include <MyString.h>

String::String() {
}

String::String(const char other_char_array[]) {
}

String::String(const String &other) {
}

String::~String() {
}

int String::length() const {
}

char String::at(int pos) const {
}

bool String::empty() const {
}

void String::set(char target_char, int pos) {
}

bool String::operator==(const String &other) const {
}

String String::operator=(const String &other) {
}

String String::operator+(const String &other) const {
}

String String::operator+(const CharSequence &other) const {
}

String String::operator+(const char char_array[]) const {
}

void String::copy_from_other(const CharSequence &other) {
}

void String::copy_from_other(const char other[]) {
}

std::istream& operator>>(std::istream &input_stream, String &string) {

    return input_stream;
}
